use std::alloc::{GlobalAlloc, Layout, System};
use std::collections::HashMap;
use std::iter::Map;
use std::ops::DerefMut;
use spin::Mutex;
use std::thread;
use std::thread::ThreadId;
use std::time::SystemTime;
use libc;
use libc::size_t;
use crate::stat_writer::MemWriter;
use crate::WRITER;

extern "C" {
	fn mh_malloc(size: size_t) -> *mut u8;
	fn mh_free(pointer: *mut u8, size: size_t);
	//fn malloc(size: usize) -> *mut u8;
	//fn free(pointer: *mut u8);

	fn init();
	fn test();
}



pub struct JukeGlobalAllocator {
	pub simple_allocator: Mutex<JukeAllocator>,
}

impl JukeGlobalAllocator {
	pub const  fn new() -> JukeGlobalAllocator {

		JukeGlobalAllocator {
			simple_allocator: Mutex::new(JukeAllocator::new())
		}
	}
	
	pub fn init_c_out(&self) {
		unsafe {
			init();
		}
	}
	
	pub fn test(&self) {
		unsafe {
			test();
		}
	}
}

unsafe impl GlobalAlloc for JukeGlobalAllocator {
	unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
		self.simple_allocator.lock().alloc(layout)
	}
	
	unsafe fn dealloc(&self, ptr: *mut u8, layout: Layout) {
		self.simple_allocator.lock().dealloc(ptr,layout);
	}
}



pub struct JukeAllocator {
	initialized: bool
}

#[inline]
fn correct_size(size: usize) -> usize {
	return  match size {
		1..=4 => 4,
		5..=8 => 8,
		9..=16 =>16,
		s => {
			if s%16 >0 {16*(1 + s/16)}
			else { s }
		}
	};
}
const MAX_ALLOC_CELL_SIZE: usize = 256;
impl JukeAllocator {
	
	pub const fn new() -> Self {
		JukeAllocator{
			initialized: false
		}
	}
	
	pub fn init(&mut self) {
		self.initialized = true;
	}
	
	unsafe fn alloc(&mut self, layout: Layout) -> *mut u8 {
		let size = correct_size(layout.size());
		
		return if size <= MAX_ALLOC_CELL_SIZE && self.initialized {
			let start = SystemTime::now();
			let result = mh_malloc(size);
			//let result = System.alloc(layout);
			let end = SystemTime::now();
			let dur = end.duration_since(start).unwrap().as_nanos();
			
			if WRITER.is_initialized() {
				WRITER.mem_writer
					.lock().expect("LOCK FAILED")
					.inc(size, dur);
			}

			
			result
		} else {
			System.alloc(layout)
		};

		let result;
		let np = layout.pad_to_align().size();
		if np<256 {
			result = mh_malloc(np)
		} else {
			result = System.alloc(layout)
		}

		if result as usize % layout.align() !=0 {
			return result;
		}
		
		return result;
		//malloc(layout.size())
	}
	
	unsafe fn dealloc(&mut self, ptr: *mut u8, layout: Layout) {
		//System.dealloc(ptr, layout);
		//return;
		
		let size = correct_size(layout.size());
		if size <= MAX_ALLOC_CELL_SIZE && self.initialized  {
			mh_free(ptr, size)
			//System.dealloc(ptr,layout)
		} else {
			System.dealloc(ptr,layout)
		}
	}
}
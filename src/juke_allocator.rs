use std::alloc::{GlobalAlloc, Layout, System};
use std::iter::Map;
use spin::Mutex;
use std::thread;
use std::thread::ThreadId;

extern "C" {
	fn mh_malloc(size: usize) -> *mut u8;
	fn mh_free(pointer: *mut u8, size: usize);
	fn malloc(size: usize) -> *mut u8;
	fn free(pointer: *mut u8);
	fn mh_print_m();
	fn init();
}

pub fn print_mem() {
	unsafe {
		mh_print_m();
	}
}

pub struct JukeGlobalAllocator {
	simple_allocator: Mutex<JukeAllocator>
}

impl JukeGlobalAllocator {
	pub const  fn new() -> JukeGlobalAllocator {

		JukeGlobalAllocator {
			simple_allocator: Mutex::new(JukeAllocator{})
		}
	}
	
	pub fn init(&self) {
		unsafe {
			init();
		}
	}
}

unsafe impl GlobalAlloc for JukeGlobalAllocator {
	unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
		self.simple_allocator.lock().alloc(layout)/*
		
		let np = layout.pad_to_align().size();
		if np < 256 {
			mh_malloc(np)
		} else {
			System.alloc(layout)
		}
*/
	}
	
	unsafe fn dealloc(&self, ptr: *mut u8, layout: Layout) {
		self.simple_allocator.lock().dealloc(ptr,layout);
	}
}

pub struct JukeAllocator {}

impl JukeAllocator {
	

	unsafe fn alloc(&mut self, layout: Layout) -> *mut u8 {
		
		
		
		let np = layout.pad_to_align().size();
		return if np < 256 {
			mh_malloc(np)
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
		let np = layout.pad_to_align().size();
		if np<256 {
			mh_free(ptr, np)
		} else {
			System.dealloc(ptr,layout)
		}
	}
}
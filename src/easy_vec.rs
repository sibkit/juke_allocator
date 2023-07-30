use std::ffi::c_void;
use std::mem::size_of;
use std::time::SystemTime;
use libc::{free, malloc, memcpy, memmove, realloc};

static INITIAL_CAPACITY: usize = 4;

pub struct EasyVec<T> {
	ptr: *mut T,
	capacity: usize,
	len: usize
}

impl <T> EasyVec<T> {
	pub fn new() -> Self {
		EasyVec {
			ptr: unsafe { malloc(size_of::<T>() * INITIAL_CAPACITY) as *mut T },
			capacity: INITIAL_CAPACITY,
			len: 0
		}
	}
	
	pub fn add_item(&mut self, item: T) {
		if self.capacity==self.len {
			self.capacity = self.capacity*2;//16/10;
			self.ptr = unsafe { realloc(self.ptr as *mut c_void,size_of::<T>()*self.capacity) as *mut T};
		}
		self.len += 1;
		let offset = size_of::<T>() * (self.len-1);
		
		let mut new_ptr = (self.ptr as usize + offset) as *mut T;
		unsafe {*new_ptr = item;}
	}
	
	pub fn remove_item(&mut self, index: usize) {
		//let size = self.len - 1 - index;
		//let start = self.ptr + index;
		//memmove(self.ptr as *mut c_void)
	}
	
	pub  fn get_mut(&mut self, index: usize) -> &mut T {
		unsafe {&mut *((self.ptr as usize + index) as *mut T) }
	}
}

impl <T> Drop for EasyVec<T> {
	fn drop(&mut self) {
		unsafe {
			free(self.ptr as *mut c_void);
		}
	}
}

#[test]
pub fn test_easy_vec() {
	
	let count = 10_000_000;
	

	
	let s2 = SystemTime::now();
	
	let mut vec = Vec::new();
	println!("std vec start capacity: {}", vec.capacity());
	let mut sc = vec.capacity();
	for i in 0..count {
		vec.push(i);
		if vec.capacity()!=sc {
			sc = vec.capacity();
			println!("std vec capacity: {}", vec.capacity());
		}
	}
	
	let e2 = SystemTime::now();
	println!("std_vec time: {}", e2.duration_since(s2).unwrap().as_millis());
	
	
	let s1 = SystemTime::now();
	
	let mut vec = EasyVec::new();
	for i in 0..count {
		vec.add_item(i);
	}
	
	let e1 = SystemTime::now();
	println!("easy_vec time: {}", e1.duration_since(s1).unwrap().as_millis());
	
	
	/*for i in 0..1_000_000 {
		if i % 10_000 == 0 {
			unsafe { println!("val: {}", *((vec.ptr as usize + i * size_of::<i32>()) as *mut i32)); }
		}
	}*/
}


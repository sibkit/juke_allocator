use std::alloc::{GlobalAlloc, Layout, System};
use std::ffi::c_void;
use std::fmt::Display;
use std::marker::PhantomData;
use std::mem::size_of;
use std::ptr::null_mut;
use libc::{c_char, free, malloc, memcpy, realloc, size_t};


pub struct HeapArray<T> {
	pub ptr: *mut T,
	pub len: usize,
}

impl <T> HeapArray<T> {
	
	pub fn new() -> Self {
		HeapArray{
			ptr: null_mut(),
			len: 0
		}
	}
	
	pub unsafe fn add_item(&mut self, item: T) {
		self.len += 1;
		
		if self.ptr.is_null() {
			self.ptr = malloc(size_of::<T>()) as *mut T;
		} else {
			self.ptr = realloc(self.ptr as *mut c_void, size_of::<T>() * self.len) as *mut T;
		}
		
		let offset = size_of::<T>() * (self.len-1);
		
		let mut new_ptr = (self.ptr as usize + offset) as *mut T;
		*new_ptr = item;
	}
	
	pub unsafe fn remove_item(&mut self, index: usize) {
		self.len -= 1;
		let ptr = malloc(self.len*size_of::<T>());
		if index>0 {
			memcpy(ptr, self.ptr as *const c_void, (index * size_of::<T>()) as size_t);
		}
		if index< (self.len - 1)  {
			memcpy(ptr.offset(index as isize), self.ptr.offset(index as isize+1) as *const c_void, (index * size_of::<T>()) as size_t);
		}
	}
}

impl <T> Drop for HeapArray<T> {
	fn drop(&mut self) {
		unsafe {
			free(self.ptr as *mut c_void);
		}
	}
}

#[test]
fn test_dyn_array() {
	let mut da: HeapArray<*mut i8> = HeapArray { ptr: null_mut(), len: 0 };
	
	let mut b: Box<[i8; 9]> = Box::new([1, 2, 3, 4, 5, 6, 7, 8, 9]);
	
	for i in 0..b.len() {
		let p = &mut b[i] as *mut i8;
		unsafe { da.add_item(p); }
	}
	
	unsafe {
		for i in 0..da.len {
			let ptr = ((da.ptr as usize) + (i * size_of::<*mut i8>())) as *mut *mut i8;
			println!("ptr: {:p}; val ptr: {:p}; val: {}", ptr, *ptr, **ptr);
		}
	}
}

#[test]
fn test_ref() {

}
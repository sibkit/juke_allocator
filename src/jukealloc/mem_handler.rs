use crate::easy_vec::EasyVec;
use crate::jukealloc::mem_pool::MemPool;

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


pub struct MemSettings {

}

pub struct MemHandler {
	pools: EasyVec<MemPool>
}

impl MemHandler {
	
	pub fn new() -> Self {
		MemHandler{
			pools: EasyVec::new()
		}
	}
	
	pub fn allocate(&mut self, size: usize) {
		let size = correct_size(size);
	}
	
	pub fn deallocate(ptr: *mut u8) {
	
	}
}
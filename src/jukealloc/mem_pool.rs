use crate::easy_vec::EasyVec;
use crate::jukealloc::mem_block::MemBlock;

static START_BLOCKS_SIZE: usize = 16;

pub struct MemPool {
	cell_size: usize,
	released_cells: EasyVec<usize>,
	mem_blocks: EasyVec<MemBlock>,
	cur_index: usize
}

impl MemPool {
	pub fn new(cell_size: usize) -> Self {
		MemPool {
			cell_size,
			released_cells: EasyVec::new(),
			mem_blocks: EasyVec::new(),
			cur_index: 0
		}
	}
	
	fn get_cur_block(&mut self) -> &mut MemBlock {
		let mut block_index=0;
		while self.cur_index < 2^(4+block_index) {
			block_index += 1;
		}
		unsafe { return self.mem_blocks.get_mut(block_index); }
	}
	
	pub fn delete_cell(&mut self, cell: *mut u8) {
	
	}
}
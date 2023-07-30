use libc::malloc;

pub(crate) struct MemBlock {
	cell_size: usize,
	cells_count: usize,
	cells: *mut u8
}

impl MemBlock {
	
	pub fn new(cell_size: usize, cells_count: usize ) -> Self {
		unsafe {
			MemBlock {
				cells_count,
				cell_size,
				cells: malloc(cell_size * cells_count) as *mut u8
			}
		}
	}
	
	pub fn get_next_cell() {
	
	}
}
use std::collections::HashMap;
use std::sync::Mutex;

pub struct SizeInfo {
	pub count: usize,
	pub time: u128,
}

pub struct StatWriter {
	pub mem_writer: Mutex<MemWriter>
}

impl StatWriter {
	pub fn print(&self) {
		self.mem_writer.lock().unwrap().print();
	}
	pub fn init(&self) {
		self.mem_writer.lock().unwrap().init();
	}
	
	pub fn is_initialized(&self) -> bool {
		self.mem_writer.lock().unwrap().is_initialized
	}

}

pub struct MemWriter {
	pub map: Option<HashMap<usize, SizeInfo>>,
	pub is_initialized: bool
}

impl MemWriter {
	pub fn init(&mut self) {
		self.map = Some(HashMap::new());
		

		let mut hm = self.map.as_mut().unwrap();
		let mut size = 1;
		while size <= 256 {
			hm.insert(size, SizeInfo { time: 0, count: 0 });
			if size <= 16 {
				size = size * 2;
			} else {
				size = size + 16;
			}
		}
		self.is_initialized = true;
		
	}
	
	pub fn inc(&mut self, size: usize, time: u128) {
		let mut si = self.map.as_mut().unwrap().get_mut(&size).unwrap();
		si.time += time;
		si.count +=1;
	}
	
	pub fn print(&self) {
		let hm = self.map.as_ref().unwrap();
		let mut vec: Vec<(&usize, &SizeInfo)> = hm.iter().collect();
		vec.sort_by(|a, b| a.0.cmp(&b.0));
		for item in vec {
			if item.1.count>0 {
				println!("size: {};	count:{};	time: {};	avg:{}", item.0, item.1.count, item.1.time, item.1.time/item.1.count as u128);
			}
			
		}
	}
}
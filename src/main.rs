use std::alloc::System;
use std::collections::HashMap;
use std::sync::Mutex;
use std::time::SystemTime;
use crate::juke_allocator::JukeGlobalAllocator;
use crate::tree_node::Tree;
use crate::stat_writer::{MemWriter, StatWriter};

mod juke_allocator;
mod tree_iterators;
mod tree_node;
mod stat_writer;
mod heap_array;
mod easy_vec;
mod jukealloc;

#[global_allocator]
static ALLOCATOR: JukeGlobalAllocator = JukeGlobalAllocator::new();

//static ALLOCATOR: System = System;
//static GLOBAL: Jemalloc = Jemalloc;
static WRITER: StatWriter = StatWriter{ mem_writer: Mutex::new(MemWriter{map:None, is_initialized: false})};


fn test_pointer() {
   let mut i = Box::new([8,16]);
   
   let mut pi: *mut i32 = &mut i[1] as *mut i32;
   unsafe {
      println!("{:?} -> {}", pi, *pi);
      pi = pi.sub(1);
      println!("{:?} -> {}", pi, *pi);
   }
   
}
fn test_mem() {
   
   let mut result = Vec::new();
   for i in 0..1 {
      result.push(Box::new(i));
   }
   println!("result: {}", result.len());
}

fn test_strings() {
   let start = SystemTime::now();
   
      let mut vec = vec![];
      for i in 0..20_000_000 {
         vec.push(i.to_string());
      }
   
   let end = SystemTime::now();
   println!("TEST STRINGS COMPLETE");
   println!("time: {}", end.duration_since(start).unwrap().as_millis());
   

   
}

fn main() {
   
   ALLOCATOR.init_c_out();
   //ALLOCATOR.test();
   //print_mem();
   //test_addresses();
  // println!("main");
   //return;

   //test_strings();
   //ALLOCATOR.simple_allocator.lock().init_statistic();
   
   WRITER.init();
   ALLOCATOR.simple_allocator.lock().init();
   
   let start = SystemTime::now();
   test_tree();
   
   let end = SystemTime::now();
   println!("TEST TREE COMPLETE");
   println!("time: {}", end.duration_since(start).unwrap().as_millis());

   WRITER.print();
   
   
   //let s: std::alloc::GlobalAlloc;
   /*
	let numbers = vec![1,2,3,4,5];
	for num in numbers {
		 println!("val: {}",num);
	}
	println!("Hello, world!");*/
}

fn test_addresses() {
   let val = 45usize;
   let b_val = Box::new(val);
   println!("{:#011x}", &val as *const usize as usize);
   //println!("{}", &b_val as *const usize as usize);
   println!("{:#011x}", &*b_val as *const usize as usize);
}

fn test_tree() {
   
   let mut tree = Tree::<String>::new();
   
   let root_id = tree.add_node("0".to_string(), None);
   for i in 0..60 {
      
      print!("put i = {}; ", i);
      let start = SystemTime::now();
     // test_tree();
      let i_id = tree.add_node(format!("{}",i), Some(root_id));
      for j in 0..1_0000 {
         let j_id = tree.add_node(format!("{} - {}",i,j), Some(i_id));
         for k in 0..1_00 {
            tree.add_node(format!("{} - {} - {}",i,j,k), Some(j_id));
         }
      }
      let end = SystemTime::now();
      println!("time: {}", end.duration_since(start).unwrap().as_millis());
   }
   
   
   
   println!("FILL COMPLETE");
   /*
   for node in tree.iter_ref(Some(root_id)) {
      println!("++ {}", &node.value);
      for j_node in tree.iter_ref(Some(node.get_id())) {
         //println!("++ {}", &j_node.value);
         for k_node in tree.iter_ref(Some(j_node.get_id())) {
           // println!("++ {}", &k_node.value);
         }
      }
   }
   */
}


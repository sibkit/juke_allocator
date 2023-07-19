use std::alloc::System;
use std::time::SystemTime;
use crate::juke_allocator::{JukeGlobalAllocator, print_mem};
use crate::tree_node::Tree;

mod juke_allocator;
mod tree_iterators;
mod tree_node;

#[global_allocator]
static ALLOCATOR: JukeGlobalAllocator = JukeGlobalAllocator::new();
//static ALLOCATOR: System = System;
//static GLOBAL: Jemalloc = Jemalloc;


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


fn main() {
   //ALLOCATOR.init();
   //print_mem();
   //test_addresses();
   
   
   let start = SystemTime::now();
   test_tree();
   println!("TEST TREE COMPLETE");
   let end = SystemTime::now();
   println!("time: {}", end.duration_since(start).unwrap().as_millis());
   
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
   for i in 0..10 {
      println!("put i = {}", i);
      let i_id = tree.add_node(format!("i -/. {} - {}", 0, i), Some(root_id));
      for j in 0..1_000 {
         let j_id = tree.add_node(format!("  j - {} - {}", 1, j), Some(i_id));
         for k in 0..1_000 {
            tree.add_node(format!("    k - {} - {}", 2, k), Some(j_id));
         }
        // println!("{}",tree.nodes_by_id.get(&j_id).unwrap().value);
      }
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
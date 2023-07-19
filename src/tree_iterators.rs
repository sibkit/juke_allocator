use std::thread::current;
use crate::tree_node::{NodeId, Tree, TreeNode};

pub struct TreeIteratorRef<'a, T> {
	tree: &'a Tree<T>,
	cur_index: usize,
	ids: Option<&'a Vec<NodeId>>
}

impl<'a, T> TreeIteratorRef<'a, T> {
	pub fn new(tree: &'a Tree<T>, parent_id: Option<NodeId>) -> Self {
		
		let ids;
		if parent_id == None {
			ids = Some(&tree.root_ids);
		}
		else {
			//println!("{}", parent_id.unwrap());
			ids = tree.ids_by_parent_id.get(&(parent_id.unwrap()));
		}
		
		TreeIteratorRef {
			tree,
			cur_index: 0,
			ids
		}
	}
}

impl<'a, T> Iterator for TreeIteratorRef<'a, T> {
	type Item = &'a TreeNode<T>;
	
	fn next(&mut self) -> Option<Self::Item> {
		//self.cur_index+=1;
		if self.cur_index >= self.tree.nodes_by_id.len()-1 {
			return None;
		}
		let result: Option<Self::Item> = if self.ids == None {
			None
		}
		else {
			let ids = self.ids.unwrap();
			let node_id = ids.get(self.cur_index).unwrap();
			if self.cur_index >= ids.len()-1 {
				return None;
			}
			Some(self.tree.nodes_by_id.get(node_id).unwrap())
		};
		self.cur_index+=1;
		result
		//let cur_node_id = self.parent.child_ids.get(self.cur_index).unwrap();
		//let cur_item = unsafe {&*cur_node_id.unwrap()};
		//self.cur_index+=1;
		//Some(cur_item)
	}
}
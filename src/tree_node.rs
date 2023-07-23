use std::collections::HashMap as AHashMap;
use std::fmt::Display;
use std::marker::PhantomData;
//use ahash::AHashMap;


pub type NodeId = *mut u8;

pub struct TreeNode<T> {
	pub value: T,
	parent_id: Option<NodeId>,
	child_ids: Vec<NodeId>
}

impl<T> TreeNode<T> {
	pub fn new(value: T, parent_id: Option<NodeId>) -> Self {
		TreeNode::<T> {
			value,
			parent_id,
			child_ids: vec![]
		}
	}
	pub fn get_id(&self) -> NodeId{
		self as *const TreeNode<T> as NodeId
	}
}

pub struct Tree<T> {
	//pub(crate) nodes_by_id: AHashMap<NodeId,TreeNode<T>>,
	//pub(crate) ids_by_parent_id: AHashMap<NodeId, Vec<NodeId>>,
	pub(crate) root_ids: Vec<NodeId>,
	phantom_data: PhantomData<T>
}

impl<T> Tree<T> where T:Display {
	pub fn new() -> Self {
		Tree {
			root_ids: vec![],
			phantom_data: Default::default(),
		}
	}
	
	#[inline]
	pub fn add_node(&mut self, value: T, parent_id: Option<NodeId>) -> NodeId {
		let mut node = Box::new(TreeNode::new(value, parent_id));
		

		

		
		let val = node.value.to_string();
		let node_id = Box::<TreeNode<T>>::into_raw(node) as NodeId;
		
		if parent_id == None {
			self.root_ids.push(node_id);
			//println!("node: {:p}, val: {} - parent: {:p}", node_id, val, &parent_id.unwrap_or(0 as NodeId));
		}
		else {

			unsafe {
				let mut pid_u = parent_id.unwrap();
				let mut parent = Box::<TreeNode<T>>::from_raw(pid_u as *mut TreeNode<T>);
				//println!("parent: {:p}, val: {} - node: {:p}, val: {}", pid_u, &parent.value, node_id, val);
				//let mut parent = &mut *(parent_id.unwrap() as *mut Box<TreeNode<T>>);
				//println!("node: {:p} - parent: {:p}", node_id, parent);
				parent.child_ids.push(node_id);
				Box::<TreeNode<T>>::into_raw(parent);
			}
			//let mut vec_opt = self.ids_by_parent_id.get_mut(&parent_id);
			//if vec_opt == None {
			//	self.ids_by_parent_id.insert(parent_id,vec![node_id]);
			//}
			//else {
			//	vec_opt.unwrap().push(node_id);
			//}
		}
		//self.nodes_by_id.insert(node_id, node);
		node_id
	}

	
	pub fn iter_mut(&mut self, parent_id: Option<NodeId>) {
	
	}
	

	
	pub fn set_parent(&mut self, node_id: NodeId, parent_id: NodeId){}
	
	pub fn get_parent_node_id(&self, node_id: NodeId) -> Option<NodeId> {
		//let node = self.nodes_by_id.get(&node_id).unwrap();
		//node.parent_id
		None
	}
}
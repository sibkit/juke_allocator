use std::collections::HashMap;
use crate::tree_iterators::TreeIteratorRef;

pub type NodeId = *const u8;

pub struct TreeNode<T> {
	pub value: T,
	parent_id: Option<NodeId>
}

impl<T> TreeNode<T> {
	pub fn new(value: T, parent_id: Option<NodeId>) -> Self {
		TreeNode::<T> {
			value,
			parent_id
		}
	}
	pub fn get_id(&self) -> NodeId{
		self as *const TreeNode<T> as NodeId
	}
}

pub struct Tree<T> {
	pub(crate) nodes_by_id: HashMap<NodeId,Box<TreeNode<T>>>,
	pub(crate) ids_by_parent_id: HashMap<NodeId, Vec<NodeId>>,
	pub(crate) root_ids: Vec<NodeId>
}

impl<T> Tree<T> {
	pub fn new() -> Self {
		Tree {
			nodes_by_id: HashMap::new(),
			ids_by_parent_id: HashMap::new(),
			root_ids: vec![],
		}
	}
	
	#[inline]
	pub fn add_node(&mut self, value: T, parent_id: Option<NodeId>) -> NodeId {
		let node = Box::new(TreeNode::new(value, parent_id));
		//self.add_boxed_node(node)
		let node_id;
		unsafe {
			let node_ref = node.as_ref();
			let ptr = node_ref as *const TreeNode<T>;
			node_id = ptr as NodeId;
		}
		
		if node.parent_id == None {
			self.root_ids.push(node_id);
		}
		else {
			let parent_id = node.parent_id.unwrap();
			let mut vec_opt = self.ids_by_parent_id.get_mut(&parent_id);
			if vec_opt == None {
				self.ids_by_parent_id.insert(parent_id,vec![node_id]);
			}
			else {
				vec_opt.unwrap().push(node_id);
			}
		}
		self.nodes_by_id.insert(node_id, node);
		node_id
	}

	
	pub fn iter_mut(&mut self, parent_id: Option<NodeId>) {
	
	}
	
	pub fn iter_ref(&self, parent_id: Option<NodeId>) -> TreeIteratorRef<T> {
		TreeIteratorRef::new(self,parent_id)
	}
	
	pub fn set_parent(&mut self, node_id: NodeId, parent_id: NodeId){}
	
	pub fn get_parent_node_id(&self, node_id: NodeId) -> Option<NodeId> {
		let node = self.nodes_by_id.get(&node_id).unwrap();
		node.parent_id
	}
}
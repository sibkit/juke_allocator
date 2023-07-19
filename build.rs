

fn main() {
	let dst = cmake::build("mem_handling");
	
	println!("cargo:rustc-link-search=native={}", dst.display());
	println!("cargo:rustc-link-lib=static=mem_handling");
}
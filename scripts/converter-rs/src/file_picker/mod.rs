use rfd::{AsyncFileDialog, FileDialog};
//use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};

fn main() {

    let file_task = AsyncFileDialog::new()
        .add_filter("Images (.png, .jpg, .jpeg, .bmp)", 
            &["png", "jpg", "jpeg", "bmp"])
        .pick_files();

    // Await somewhere else
    let _files = execute(async {
        let file = file_task.await;

        if let Some(file) = file {
            for f in file {
                // If you are on native platform you can just get the path
                #[cfg(not(target_arch = "wasm32"))]
                println!("{:?}", f.path());

                // If you care about wasm support you just read() the file
                f.read().await;
            }
        }
    });

    loop {
        // std::println!("PRINTED");
    }
}

pub fn pick_blocking() -> Option<std::vec::Vec<std::path::PathBuf>> {
    let files = FileDialog::new()
        .add_filter("Images (.png, .jpg, .jpeg, .bmp)", 
            &["png", "jpg", "jpeg", "bmp"])
        .pick_files();

    // files.map(|x| {
    //     std::println!("{:?}", x);
    //     for f in x {
    //         std::println!("{:?}", f);
    //     }
    // });

    return files;
}

use std::future::Future;

#[cfg(not(target_arch = "wasm32"))]
fn execute<F: Future<Output = ()> + Send + 'static>(f: F) {
    // this is stupid... use any executor of your choice instead
    std::thread::spawn(move || futures::executor::block_on(f));
}
#[cfg(target_arch = "wasm32")]
fn execute<F: Future<Output = ()> + 'static>(f: F) {
    wasm_bindgen_futures::spawn_local(f);
}

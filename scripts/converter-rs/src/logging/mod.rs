pub struct Logger {
    verbosity : u8,
    lines : std::vec::Vec<&'static str>,
}

impl Logger {
    pub fn new(verbosity: u8) -> Self {
        let lines = vec!["Logger Initialized"];

        Self {
            verbosity,
            lines
        } 
    }    

    pub fn log(&mut self, line: &'static str) {
        let lines = &mut self.lines;
        lines.push(line.clone());
        println!("Logged: {}", line);
    }
}

use std::env;

fn print_help() {
    println!("Version {0}\n", "1.0.5"); // TODO: use a versioning file and macro

    println!("Usage:");
    println!("  numconvert [prefix]<value>[postfix]");

    println!("\nNo prefix/postfix:");
    println!("            to indicate a decimal value");

    println!("\nPrefixes:");
    println!("  0x or 0X, to indicate a hexadecimal value");

    println!("\nPostfixes:");
    println!("  b or B,   to indicate a binary value");
    println!("  h or H,   to indicate a hexadecimal value\n");
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        print_help();
        std::process::exit(-1);
    }

    let raw = String::from(&args[1]).to_lowercase();

    if raw.starts_with("0x") {
        // It's hexadecimal, print the results
        let number = raw.trim_start_matches("0x");
        let my_int = i64::from_str_radix(&number, 16);
        println!("{:?}", my_int);
    }




}

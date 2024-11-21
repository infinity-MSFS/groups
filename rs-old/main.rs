mod types;

use flate2::{write::GzEncoder, Compression};
use rmp_serde::encode;
use serde_json;
use std::{collections::HashMap, env, fs, io::Write, process};
use types::GroupData;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 3 {
        eprintln!("Usage: {} <input_json> <output_bin>", args[0]);
        process::exit(1);
    }

    let input_path = &args[1];
    let output_path = &args[2];

    let raw_json = match fs::read_to_string(input_path) {
        Ok(res) => res,
        Err(e) => {
            eprintln!("error reading JSON file: {}", e);
            process::exit(1);
        }
    };

    let bin_data = match write_bin(raw_json) {
        Ok(data) => data,
        Err(e) => {
            eprintln!("error converting from JSON to BIN: {}", e);
            process::exit(1);
        }
    };

    if let Err(e) = fs::write(output_path, bin_data) {
        eprintln!("errror writing BIN file: {}", e);
        process::exit(1);
    }
}

fn write_bin(raw_json: String) -> Result<Vec<u8>, String> {
    let data: HashMap<String, GroupData> = serde_json::from_str(&raw_json).unwrap();

    let mut buffer: Vec<u8> = Vec::new();
    match encode::write(&mut buffer, &data) {
        Ok(_) => {
            let mut encoder = GzEncoder::new(Vec::new(), Compression::best());
            match encoder.write_all(&buffer) {
                Ok(_) => match encoder.finish() {
                    Ok(compressed_data) => Ok(compressed_data),
                    Err(e) => Err(format!("Error closing encoder: {}", e)),
                },
                Err(e) => Err(format!("error writing from buffer: {}", e)),
            }
        }
        Err(e) => Err(format!("Error encoding: {}", e)),
    }
}

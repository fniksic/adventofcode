use std::{
    collections::{BinaryHeap, HashMap},
    env,
    fs::File,
    io::{BufRead, BufReader},
};

fn main() {
    let mut args = env::args();
    args.next();

    let file = args.next().expect("Expected an input file as an argument.");
    let file = File::open(file).expect("Failed to open input file.");
    let reader = BufReader::new(file);

    let mut fst_heap = BinaryHeap::new();
    let mut snd_heap = BinaryHeap::new();
    let mut counts = HashMap::new();
    for line in reader.lines() {
        let line = line.unwrap();
        let mut tokens = line.split_whitespace();

        fst_heap.push(tokens.next().unwrap().parse::<i32>().unwrap());

        let snd_num = tokens.next().unwrap().parse::<i32>().unwrap();
        snd_heap.push(snd_num);
        match counts.get_mut(&snd_num) {
            Some(count) => {
                *count += 1;
            }
            None => {
                counts.insert(snd_num, 1);
            }
        }
    }

    let mut distance = 0;
    let mut similarity = 0;
    while !fst_heap.is_empty() {
        assert!(!snd_heap.is_empty());
        let fst_num = fst_heap.pop().unwrap();
        distance += fst_num.abs_diff(snd_heap.pop().unwrap());
        similarity += fst_num * counts.get(&fst_num).unwrap_or(&0);
    }

    println!("Distance: {distance}");
    println!("Similarity: {similarity}");
}

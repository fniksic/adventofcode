use std::fs;

use regex::Regex;

fn sum_muls(memory: &str) -> i32 {
    let re = Regex::new(r"mul\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\)").unwrap();
    re.captures_iter(memory)
        .map(|caps| {
            let fst = caps[1].parse::<i32>().unwrap();
            let snd = caps[2].parse::<i32>().unwrap();
            fst * snd
        })
        .sum()
}

fn sum_conditional_muls(memory: &str) -> i32 {
    let re = Regex::new(
        r"mul\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\)|do\(\)|don't\(\)",
    )
    .unwrap();
    let mut disabled = false;
    let mut sum = 0;
    for caps in re.captures_iter(memory) {
        if &caps[0] == "do()" {
            disabled = false;
            continue;
        }
        if &caps[0] == "don't()" {
            disabled = true;
            continue;
        }
        if disabled {
            continue;
        }
        let fst = caps[1].parse::<i32>().unwrap();
        let snd = caps[2].parse::<i32>().unwrap();
        sum += fst * snd;
    }
    sum
}

fn main() {
    let memory = fs::read_to_string("input")
        .expect("Failed to read from the input file.");
    let sum = sum_muls(&memory);
    let conditional_sum = sum_conditional_muls(&memory);
    println!("Sum: {sum}");
    println!("Conditional sum: {conditional_sum}");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn sum_muls_works() {
        let memory = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
        assert_eq!(sum_muls(memory), 161);
    }

    #[test]
    fn sum_conditional_muls_works() {
        let memory = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
        assert_eq!(sum_conditional_muls(memory), 48);
    }
}

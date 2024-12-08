use std::{
    fs::File,
    io::{BufRead, BufReader},
};

fn parse(line: &str) -> (i64, Vec<i64>) {
    let (val_part, nums_part) = line.split_once(':').unwrap();
    let val = val_part.parse().unwrap();
    let nums = nums_part
        .split_ascii_whitespace()
        .map(|num_part| num_part.parse().unwrap())
        .collect();
    (val, nums)
}

fn can_produce(val: i64, nums: &[i64]) -> bool {
    assert!(!nums.is_empty());
    let len = nums.len();
    let last = *nums.last().unwrap();
    if len == 1 {
        return val == last;
    }
    (val % last == 0 && can_produce(val / last, &nums[..len - 1]))
        || (val >= last && can_produce(val - last, &nums[..len - 1]))
}

fn pow_ten(n: i64) -> i64 {
    // Largest power of 10 that fits in i64.
    assert!(n < 1_000_000_000_000_000_000);
    let mut pow: i64 = 1;
    while pow <= n {
        pow *= 10;
    }
    pow
}

fn can_produce_with_concat(val: i64, nums: &[i64]) -> bool {
    assert!(!nums.is_empty());
    let len = nums.len();
    let last = *nums.last().unwrap();
    if len == 1 {
        return val == last;
    }
    let pow_ten_last = pow_ten(last);
    (val % pow_ten_last == last
        && can_produce_with_concat(val / pow_ten_last, &nums[..len - 1]))
        || (val % last == 0
            && can_produce_with_concat(val / last, &nums[..len - 1]))
        || (val >= last
            && can_produce_with_concat(val - last, &nums[..len - 1]))
}

fn total_calibration(lines: impl Iterator<Item = String>) -> (i64, i64) {
    let mut total_cal = 0;
    let mut total_cal_with_concat = 0;
    for (val, nums) in lines.map(|line| parse(&line)) {
        if can_produce(val, &nums) {
            total_cal += val;
            total_cal_with_concat += val;
        } else if can_produce_with_concat(val, &nums) {
            total_cal_with_concat += val;
        }
    }
    (total_cal, total_cal_with_concat)
}

fn main() {
    let file = File::open("input").expect("Failed to open the input file.");
    let reader = BufReader::new(file);
    let (total_cal, total_cal_with_concat) =
        total_calibration(reader.lines().map(|line| line.unwrap()));
    println!("Total calibration: {total_cal}");
    println!("Total calibration with concatenation: {total_cal_with_concat}");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_is_correct() {
        assert_eq!(parse("190: 10 19"), (190, vec![10, 19]));
        assert_eq!(parse("3267: 81 40 27"), (3267, vec![81, 40, 27]));
        assert_eq!(parse("83: 17 5"), (83, vec![17, 5]));
        assert_eq!(parse("156: 15 6"), (156, vec![15, 6]));
        assert_eq!(parse("7290: 6 8 6 15"), (7290, vec![6, 8, 6, 15]));
        assert_eq!(parse("161011: 16 10 13"), (161011, vec![16, 10, 13]));
        assert_eq!(parse("192: 17 8 14"), (192, vec![17, 8, 14]));
        assert_eq!(parse("21037: 9 7 18 13"), (21037, vec![9, 7, 18, 13]));
        assert_eq!(parse("292: 11 6 16 20"), (292, vec![11, 6, 16, 20]));
    }

    #[test]
    fn can_produce_is_correct() {
        assert!(can_produce(190, &[10, 19]));
        assert!(can_produce(3267, &[81, 40, 27]));
        assert!(!can_produce(83, &[17, 5]));
        assert!(!can_produce(156, &[15, 6]));
        assert!(!can_produce(7290, &[6, 8, 6, 15]));
        assert!(!can_produce(161011, &[16, 10, 13]));
        assert!(!can_produce(192, &[17, 8, 14]));
        assert!(!can_produce(21037, &[9, 7, 18, 13]));
        assert!(can_produce(292, &[11, 6, 16, 20]));
    }

    #[test]
    fn pow_ten_is_correct() {
        assert_eq!(pow_ten(1), 10);
        assert_eq!(pow_ten(8), 10);
        assert_eq!(pow_ten(987_654_321), 1_000_000_000);
        assert_eq!(pow_ten(235_767_423_789_123_182), 1_000_000_000_000_000_000);
    }

    #[test]
    fn can_produce_with_concat_is_correct() {
        assert!(can_produce_with_concat(190, &[10, 19]));
        assert!(can_produce_with_concat(3267, &[81, 40, 27]));
        assert!(!can_produce_with_concat(83, &[17, 5]));
        assert!(can_produce_with_concat(156, &[15, 6]));
        assert!(can_produce_with_concat(7290, &[6, 8, 6, 15]));
        assert!(!can_produce_with_concat(161011, &[16, 10, 13]));
        assert!(can_produce_with_concat(192, &[17, 8, 14]));
        assert!(!can_produce_with_concat(21037, &[9, 7, 18, 13]));
        assert!(can_produce_with_concat(292, &[11, 6, 16, 20]));
    }

    #[test]
    fn total_calibration_is_correct() {
        let lines = [
            "190: 10 19",
            "3267: 81 40 27",
            "83: 17 5",
            "156: 15 6",
            "7290: 6 8 6 15",
            "161011: 16 10 13",
            "192: 17 8 14",
            "21037: 9 7 18 13",
            "292: 11 6 16 20",
        ]
        .iter()
        .map(|line| String::from(*line));
        assert_eq!(total_calibration(lines), (3749, 11387));
    }
}

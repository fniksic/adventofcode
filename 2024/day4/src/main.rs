use std::{
    fs::File,
    io::{BufRead, BufReader},
};

struct Direction(isize, isize);

fn count_xmas(lines: &[String]) -> i32 {
    let word = "XMAS";
    let directions = [
        Direction(0, 1),
        Direction(1, 1),
        Direction(1, 0),
        Direction(1, -1),
        Direction(0, -1),
        Direction(-1, -1),
        Direction(-1, 0),
        Direction(-1, 1),
    ];
    let match_in_direction = |mut i: usize, mut j: usize, d: &Direction| {
        if lines[i].as_bytes()[j] != word.as_bytes()[0] {
            return false;
        }
        for b in word.bytes().skip(1) {
            match i.checked_add_signed(d.0) {
                Some(new_i) => i = new_i,
                None => return false,
            }
            match j.checked_add_signed(d.1) {
                Some(new_j) => j = new_j,
                None => return false,
            }
            if !(i < lines.len()
                && j < lines[i].as_bytes().len()
                && lines[i].as_bytes()[j] == b)
            {
                return false;
            }
        }
        true
    };

    let mut count = 0;
    for i in 0..lines.len() {
        for j in 0..lines[i].as_bytes().len() {
            for d in directions.iter() {
                if match_in_direction(i, j, d) {
                    count += 1;
                }
            }
        }
    }
    count
}

fn count_x_shaped_mas(lines: &[String]) -> i32 {
    let patterns = [
        ["M.M", ".A.", "S.S"],
        ["M.S", ".A.", "M.S"],
        ["S.S", ".A.", "M.M"],
        ["S.M", ".A.", "S.M"],
    ];
    let matches_pattern = |i: usize, j: usize, p: &[&str]| {
        for k in 0..p.len() {
            if i + k >= lines.len() {
                return false;
            }
            for l in 0..p[k].as_bytes().len() {
                if p[k].as_bytes()[l] == b'.' {
                    continue;
                }
                if j + l >= lines[i + k].as_bytes().len()
                    || lines[i + k].as_bytes()[j + l] != p[k].as_bytes()[l]
                {
                    return false;
                }
            }
        }
        true
    };
    let mut count = 0;
    for i in 0..lines.len() {
        for j in 0..lines[i].as_bytes().len() {
            for p in patterns.iter() {
                if matches_pattern(i, j, p) {
                    count += 1;
                    break;
                }
            }
        }
    }
    count
}

fn main() {
    let file = File::open("input").expect("Failed to open input file.");
    let reader = BufReader::new(file);
    let lines: Vec<String> = reader.lines().map(|line| line.unwrap()).collect();
    let count = count_xmas(&lines);
    println!("Count XMAS: {count}");
    let count = count_x_shaped_mas(&lines);
    println!("Count X-MAS: {count}");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn count_xmas_is_correct() {
        let lines = [
            String::from("MMMSXXMASM"),
            String::from("MSAMXMSMSA"),
            String::from("AMXSXMAAMM"),
            String::from("MSAMASMSMX"),
            String::from("XMASAMXAMM"),
            String::from("XXAMMXXAMA"),
            String::from("SMSMSASXSS"),
            String::from("SAXAMASAAA"),
            String::from("MAMMMXMMMM"),
            String::from("MXMXAXMASX"),
        ];
        assert_eq!(count_xmas(&lines), 18);
    }

    #[test]
    fn count_x_shaped_mas_is_correct() {
        let lines = [
            String::from("MMMSXXMASM"),
            String::from("MSAMXMSMSA"),
            String::from("AMXSXMAAMM"),
            String::from("MSAMASMSMX"),
            String::from("XMASAMXAMM"),
            String::from("XXAMMXXAMA"),
            String::from("SMSMSASXSS"),
            String::from("SAXAMASAAA"),
            String::from("MAMMMXMMMM"),
            String::from("MXMXAXMASX"),
        ];
        assert_eq!(count_x_shaped_mas(&lines), 9);
    }
}

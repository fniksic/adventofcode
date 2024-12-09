use std::{
    collections::{HashMap, HashSet},
    fs::File,
    io::{BufRead, BufReader},
};

fn count_antinodes(map: &[Vec<u8>]) -> (usize, usize) {
    assert!(!map.is_empty() && !map[0].is_empty());
    let (height, width) = (map.len(), map[0].len());

    let mut antennas = HashMap::<u8, Vec<(usize, usize)>>::new();
    let mut antinodes = HashSet::<(usize, usize)>::new();
    let mut extended_antinodes = HashSet::<(usize, usize)>::new();
    for (i, line) in map.iter().enumerate() {
        for (j, location) in line.iter().enumerate() {
            if *location == b'.' {
                continue;
            }
            antennas
                .entry(*location)
                .and_modify(|prev_antennas| {
                    for (prev_i, prev_j) in &*prev_antennas {
                        let (prev_i, prev_j) = (*prev_i, *prev_j);
                        // Antinode locations:
                        // (2 * prev_i - i, 2 * prev_j - j)
                        // (2 * i - prev_i, 2 * j - prev_j)
                        if 2 * prev_i >= i
                            && 2 * prev_j >= j
                            && 2 * prev_i - i < height
                            && 2 * prev_j - j < width
                        {
                            antinodes.insert((2 * prev_i - i, 2 * prev_j - j));
                        }
                        if 2 * i >= prev_i
                            && 2 * j >= prev_j
                            && 2 * i - prev_i < height
                            && 2 * j - prev_j < width
                        {
                            antinodes.insert((2 * i - prev_i, 2 * j - prev_j));
                        }

                        // Extended antinodes:
                        //   (prev_i, prev_j) + n (i - prev_i, j - prev_j)
                        // for all integer n. Break it down into adding and
                        // subtracting as long as we're on the map.
                        let mut curr_i = prev_i;
                        let mut curr_j = prev_j;
                        while curr_i + i >= prev_i
                            && curr_j + j >= prev_j
                            && curr_i + i - prev_i < height
                            && curr_j + j - prev_j < width
                        {
                            curr_i = curr_i + i - prev_i;
                            curr_j = curr_j + j - prev_j;
                            extended_antinodes.insert((curr_i, curr_j));
                        }
                        let mut curr_i = i;
                        let mut curr_j = j;
                        while curr_i + prev_i >= i
                            && curr_j + prev_j >= j
                            && curr_i + prev_i - i < height
                            && curr_j + prev_j - j < width
                        {
                            curr_i = curr_i + prev_i - i;
                            curr_j = curr_j + prev_j - j;
                            extended_antinodes.insert((curr_i, curr_j));
                        }
                    }
                    prev_antennas.push((i, j));
                })
                .or_insert(vec![(i, j)]);
        }
    }

    (antinodes.len(), extended_antinodes.len())
}

fn main() {
    let file = File::open("input").expect("Failed to open the input file.");
    let reader = BufReader::new(file);
    let map: Vec<Vec<u8>> = reader
        .lines()
        .map(|line| Vec::from(line.unwrap().as_bytes()))
        .collect();
    let (antinodes, extended_antinodes) = count_antinodes(&map);
    println!("Total antinodes: {antinodes}");
    println!("Total extended antinodes: {extended_antinodes}");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn count_antinodes_is_correct() {
        let map = [
            "............",
            "........0...",
            ".....0......",
            ".......0....",
            "....0.......",
            "......A.....",
            "............",
            "............",
            "........A...",
            ".........A..",
            "............",
            "............",
        ]
        .map(|line| Vec::from(line.as_bytes()));

        assert_eq!(count_antinodes(&map), (14, 34));
    }
}

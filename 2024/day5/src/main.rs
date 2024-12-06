use std::{
    cmp::Ordering,
    collections::{HashMap, HashSet},
    fs::File,
    io::{BufRead, BufReader},
};

// Maps a page to a set of pages that appear before it.
type PagesBefore = HashMap<i32, HashSet<i32>>;

fn read_pages_before(lines: &mut impl Iterator<Item = String>) -> PagesBefore {
    let mut pages_before = PagesBefore::new();
    for line in lines.take_while(|line| !line.is_empty()) {
        let (lhs, rhs) = line
            .split_once('|')
            .expect("Improperly formatted ordering rule!");
        let lhs = lhs.parse::<i32>().unwrap();
        let rhs = rhs.parse::<i32>().unwrap();
        pages_before
            .entry(rhs)
            .and_modify(|set| {
                set.insert(lhs);
            })
            .or_insert(HashSet::from([lhs]));
    }
    pages_before
}

fn read_update(line: String) -> Vec<i32> {
    line.split(',')
        .map(|num| num.parse::<i32>().unwrap())
        .collect()
}

fn is_correctly_ordered(update: &[i32], pages_before: &PagesBefore) -> bool {
    // No need to check transitive ordering rules. If a < b < c and c appears
    // before a in an update, one of the following is true:
    //
    // * b doesn't appear in the update so the rule should be ignored
    // * b appears before a, more immediately breaking the rule a < b
    // * b appears after c, more immediately breaking the rule b < c
    let mut should_not_appear = HashSet::<i32>::new();
    for page in update {
        if should_not_appear.contains(page) {
            return false;
        }
        if let Some(pages) = pages_before.get(page) {
            should_not_appear.extend(pages);
        }
    }
    true
}

fn main() {
    let file = File::open("input").expect("Failed to open input file.");
    let reader = BufReader::new(file);
    let mut lines_iter = reader.lines().map(|line| line.unwrap());
    let pages_before = read_pages_before(&mut lines_iter);
    let (sum_correct, sum_incorrect) = lines_iter.map(read_update).fold(
        (0, 0),
        |(sum_correct, sum_incorrect), mut update| {
            if is_correctly_ordered(&update, &pages_before) {
                return (sum_correct + update[update.len() / 2], sum_incorrect);
            }
            update.sort_by(|a, b| {
                if let Some(pages) = pages_before.get(b) {
                    if pages.contains(a) {
                        return Ordering::Less;
                    }
                }
                if let Some(pages) = pages_before.get(a) {
                    if pages.contains(b) {
                        return Ordering::Greater;
                    }
                }
                Ordering::Equal
            });
            (sum_correct, sum_incorrect + update[update.len() / 2])
        },
    );
    println!("Sum correct: {sum_correct}\nSum incorrect: {sum_incorrect}");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn read_pages_before_correctly_parses_rules() {
        let mut input = [
            "47|53", "97|13", "97|61", "97|47", "75|29", "61|13", "75|53",
            "29|13", "97|29", "53|29", "61|53", "97|53", "61|29", "47|13",
            "75|47", "97|75", "47|61", "75|61", "47|29", "75|13", "53|13",
        ]
        .iter()
        .map(|line| String::from(*line));
        assert_eq!(
            read_pages_before(&mut input),
            PagesBefore::from([
                (13, HashSet::from([29, 47, 53, 61, 75, 97])),
                (29, HashSet::from([47, 53, 61, 75, 97])),
                (47, HashSet::from([75, 97])),
                (53, HashSet::from([47, 61, 75, 97])),
                (61, HashSet::from([47, 75, 97])),
                (75, HashSet::from([97]))
            ])
        );
    }

    #[test]
    fn read_pages_before_only_processes_up_to_empty_line() {
        let mut input = ["", "remaining", "lines"]
            .iter()
            .map(|line| String::from(*line));
        let _ = read_pages_before(&mut input);
        assert_eq!(
            input.collect::<Vec<String>>(),
            Vec::from([String::from("remaining"), String::from("lines")])
        );
    }

    #[test]
    fn is_correctly_ordered_identifies_correctly_ordered_updates() {
        let pages_before = PagesBefore::from([
            (13, HashSet::from([29, 47, 53, 61, 75, 97])),
            (29, HashSet::from([47, 53, 61, 75, 97])),
            (47, HashSet::from([75, 97])),
            (53, HashSet::from([47, 61, 75, 97])),
            (61, HashSet::from([47, 75, 97])),
            (75, HashSet::from([97])),
        ]);

        assert!(is_correctly_ordered(&[75, 47, 61, 53, 29], &pages_before));
        assert!(is_correctly_ordered(&[97, 61, 53, 29, 13], &pages_before));
        assert!(is_correctly_ordered(&[75, 29, 13], &pages_before));
        assert!(!is_correctly_ordered(&[75, 97, 47, 61, 53], &pages_before));
        assert!(!is_correctly_ordered(&[61, 13, 29], &pages_before));
        assert!(!is_correctly_ordered(&[97, 13, 75, 29, 47], &pages_before));
    }
}

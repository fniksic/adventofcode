use std::{
    fs::File,
    io::{BufRead, BufReader},
};

struct Report {
    levels: Vec<i32>,
}

struct ReportSafety {
    sgn_sum: i32,
    diffs_are_safe: bool,
    is_safe: bool,
}

fn sgn_sum_matches_len(sgn_sum: i32, len: usize) -> bool {
    usize::try_from((sgn_sum + sgn_sum.signum()).abs()).unwrap_or(0) == len
}

// Returns cumulative safety reports when iterating over 2-sized windows
// of a slice.
fn get_report_safeties(
    lvl_pairs: &mut dyn DoubleEndedIterator<Item = &[i32]>,
) -> Vec<ReportSafety> {
    let mut safeties = Vec::new();

    // Add a sentinel
    safeties.push(ReportSafety {
        sgn_sum: 0,
        diffs_are_safe: true,
        is_safe: true,
    });

    let mut sgn_sum = 0;
    let mut diffs_are_safe = true;
    for (i, w) in lvl_pairs.enumerate() {
        sgn_sum += (w[1] - w[0]).signum();
        let ascends_or_descends = sgn_sum_matches_len(sgn_sum, i + 2);
        let diff = w[1].abs_diff(w[0]);
        let is_diff_safe = 1 <= diff && diff <= 3;
        diffs_are_safe &= is_diff_safe;
        safeties.push(ReportSafety {
            sgn_sum,
            diffs_are_safe,
            is_safe: ascends_or_descends && diffs_are_safe,
        });
    }
    safeties
}

impl Report {
    fn new(line: String) -> Report {
        Report {
            levels: line
                .split_whitespace()
                .map(|lvl| lvl.parse::<i32>().unwrap())
                .collect(),
        }
    }

    fn is_safe(&self) -> bool {
        if self.levels.len() <= 1 {
            return true;
        }
        let safeties = get_report_safeties(&mut self.levels.windows(2));
        assert_eq!(safeties.len(), self.levels.len());
        safeties.last().unwrap().is_safe
    }

    fn is_weakly_safe(&self) -> bool {
        if self.levels.len() <= 1 {
            return true;
        }

        let safeties = get_report_safeties(&mut self.levels.windows(2));
        assert_eq!(safeties.len(), self.levels.len());

        // If safe without ignoring anything
        if safeties.last().unwrap().is_safe {
            return true;
        }

        let rev_safeties =
            get_report_safeties(&mut self.levels.windows(2).rev());
        assert_eq!(rev_safeties.len(), self.levels.len());

        // Can I drop the first one?
        if rev_safeties[rev_safeties.len() - 2].is_safe {
            return true;
        }

        // Can I drop the last one?
        if safeties[safeties.len() - 2].is_safe {
            return true;
        }

        for i in 1..self.levels.len() - 1 {
            // Can I drop the i-th?
            let prev_lvl = self.levels[i - 1];
            let next_lvl = self.levels[i + 1];
            let sgn = (next_lvl - prev_lvl).signum();
            let sgn_sum = safeties[i - 1].sgn_sum
                + sgn
                + rev_safeties[rev_safeties.len() - i - 2].sgn_sum;
            let ascends_or_descends =
                sgn_sum_matches_len(sgn_sum, self.levels.len() - 1);
            let diff = next_lvl.abs_diff(prev_lvl);
            let diffs_are_safe = safeties[i - 1].diffs_are_safe
                && rev_safeties[rev_safeties.len() - i - 2].diffs_are_safe
                && 1 <= diff
                && diff <= 3;
            if diffs_are_safe && ascends_or_descends {
                return true;
            }
        }

        // Not safe even if we drop a level
        return false;
    }
}

fn main() {
    let file = File::open("input").expect("Failed to open input file.");
    let reader = BufReader::new(file);

    let (safe, weakly_safe) = reader
        .lines()
        .map(|line| Report::new(line.unwrap()))
        .fold((0, 0), |(safe, weakly_safe), report| {
            (
                safe + (if report.is_safe() { 1 } else { 0 }),
                weakly_safe + (if report.is_weakly_safe() { 1 } else { 0 }),
            )
        });
    println!("Safe reports: {safe}");
    println!("Weakly safe reports: {weakly_safe}");
}

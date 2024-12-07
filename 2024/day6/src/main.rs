use std::{
    fs::File,
    io::{BufRead, BufReader},
    ops::Neg,
};

#[derive(PartialEq, Debug, Clone, Copy)]
struct Direction {
    i: isize,
    j: isize,
}

impl Direction {
    fn rotate_left(&self) -> Direction {
        Direction {
            i: -self.j,
            j: self.i,
        }
    }

    fn rotate_right(&self) -> Direction {
        Direction {
            i: self.j,
            j: -self.i,
        }
    }
}

impl Neg for Direction {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Direction {
            i: -self.i,
            j: -self.j,
        }
    }
}

impl Into<u8> for Direction {
    fn into(self) -> u8 {
        match self {
            Direction { i: 0, j: 1 } => b'>',
            Direction { i: 1, j: 0 } => b'v',
            Direction { i: 0, j: -1 } => b'<',
            Direction { i: -1, j: 0 } => b'^',
            _ => panic!("Unexpected direction: {:?}", self),
        }
    }
}

impl TryFrom<u8> for Direction {
    type Error = ();

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            b'>' => Ok(Direction { i: 0, j: 1 }),
            b'v' => Ok(Direction { i: 1, j: 0 }),
            b'<' => Ok(Direction { i: 0, j: -1 }),
            b'^' => Ok(Direction { i: -1, j: 0 }),
            _ => Err(()),
        }
    }
}

fn find_start(map: &[Vec<u8>]) -> Option<(usize, usize, Direction)> {
    for i in 0..map.len() {
        for j in 0..map[i].len() {
            if let Ok(direction) = Direction::try_from(map[i][j]) {
                return Some((i, j, direction));
            }
        }
    }
    None
}

fn count_visited_positions_and_looping_obstructions(
    mut map: Vec<Vec<u8>>,
) -> (i32, i32) {
    assert!(!map.is_empty() && !map[0].is_empty());
    let (height, width) = (map.len(), map[0].len());
    let (start_i, start_j, start_d) = find_start(&map).unwrap();

    let take_step = |i: usize, j: usize, d: Direction| {
        let next_i;
        match i.checked_add_signed(d.i) {
            Some(i) => next_i = i,
            None => return None,
        }
        let next_j;
        match j.checked_add_signed(d.j) {
            Some(j) => next_j = j,
            None => return None,
        }
        if next_i < height && next_j < width {
            Some((next_i, next_j))
        } else {
            None
        }
    };

    // Current position and direction
    let mut i = start_i;
    let mut j = start_j;
    let mut d = start_d;

    // `directions[i][j]` is a stack of directions used to get into position
    // `(i,j)`, with the top of the stack being the most recent direction.
    let mut directions: Vec<Vec<Vec<u8>>> = vec![vec![vec![]; width]; height];
    directions[i][j].push(d.into());

    // When checking an added obstruction, a bookmarked position to get back to.
    // It is not enough to just mark the coordinates, but also the number of
    // directions on the stack at the bookmarked position. We may need to step
    // through `(back_i, back_j)` several times while backtracking to the
    // original position.
    let mut back_i = height;
    let mut back_j = width;
    let mut back_directions_count = 0;
    let mut checking_obstruction = false;

    // Count the starting position as visited.
    let mut visited_positions = 1;
    let mut looping_obstructions = 0;

    loop {
        let mut backtrack = false;
        let mut next_i = height;
        let mut next_j = width;
        if let Some((n_i, n_j)) = take_step(i, j, d) {
            next_i = n_i;
            next_j = n_j;
        } else {
            // We step out of the map. We're either done or need to backtrack.
            if !checking_obstruction {
                break;
            }
            backtrack = true;
        };
        assert!(checking_obstruction || (next_i < height && next_j < width));
        // See if we can close a loop by placing an obstruction in front. Only
        // check positions that haven't been checked before (i.e., positions
        // without saved directions).
        if !checking_obstruction
            && directions[next_i][next_j].is_empty()
            && map[next_i][next_j] == b'.'
        {
            // We need a different obstruction marker from '#' to account for
            // rotating in place facing obstructions in multiple directions.
            map[next_i][next_j] = b'O';
            back_i = i;
            back_j = j;
            back_directions_count = directions[i][j].len();
            checking_obstruction = true;
        }
        assert!(backtrack || (next_i < height && next_j < width));
        if !backtrack
            && (map[next_i][next_j] == b'#' || map[next_i][next_j] == b'O')
        {
            d = d.rotate_right();
            continue;
        }
        // If we enter `(next_i, next_j)` from a previously stored direction,
        // we've closed a loop.
        if !backtrack && directions[next_i][next_j].contains(&d.into()) {
            // There aren't supposed to be any loops if we haven't added an
            // additional obstruction.
            assert!(checking_obstruction);
            looping_obstructions += 1;
            backtrack = true;
        }
        if backtrack {
            assert!(checking_obstruction);
            // First backtrack to `(back_i, back_j)` with the directions stack
            // of size `back_directions_count` by popping directions and taking
            // steps backwards along the way.
            while !(i == back_i
                && j == back_j
                && directions[i][j].len() == back_directions_count)
            {
                assert!(!directions[i][j].is_empty());
                let prev_d: Direction =
                    directions[i][j].pop().unwrap().try_into().unwrap();
                let (prev_i, prev_j) = take_step(i, j, -prev_d).unwrap();
                next_i = i;
                next_j = j;
                i = prev_i;
                j = prev_j;
                d = prev_d;
                assert!(i < height && j < width);
            }
            // To account for in-place rotations, rotate left until we see the
            // added obstruction.
            while next_i >= height
                || next_j >= width
                || map[next_i][next_j] != b'O'
            {
                d = d.rotate_left();
                let (n_i, n_j) = take_step(i, j, d).unwrap();
                next_i = n_i;
                next_j = n_j;
            }
            map[next_i][next_j] = b'.';
            checking_obstruction = false;
        }
        assert!(next_i < height && next_j < width);
        if directions[next_i][next_j].is_empty() && !checking_obstruction {
            visited_positions += 1;
        }
        directions[next_i][next_j].push(d.into());
        i = next_i;
        j = next_j;
    }

    (visited_positions, looping_obstructions)
}

fn main() {
    let file = File::open("input").expect("Failed to open the input file.");
    let reader = BufReader::new(file);
    let map: Vec<Vec<u8>> = reader
        .lines()
        .map(|line| line.unwrap().into_bytes())
        .collect();
    let (visited_positions, looping_obstructions) =
        count_visited_positions_and_looping_obstructions(map);
    println!("Visited positions: {visited_positions}");
    println!("Looping obstructions: {looping_obstructions}");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn rotate_right_works_correctly() {
        assert_eq!(
            Direction { i: 0, j: 1 }.rotate_right(),
            Direction { i: 1, j: 0 }
        );
        assert_eq!(
            Direction { i: 1, j: 0 }.rotate_right(),
            Direction { i: 0, j: -1 }
        );
        assert_eq!(
            Direction { i: 0, j: -1 }.rotate_right(),
            Direction { i: -1, j: 0 }
        );
        assert_eq!(
            Direction { i: -1, j: 0 }.rotate_right(),
            Direction { i: 0, j: 1 }
        );
    }

    #[test]
    fn rotate_left_is_reverse_of_rotate_right() {
        assert_eq!(
            Direction { i: 0, j: 1 }.rotate_left().rotate_right(),
            Direction { i: 0, j: 1 }
        );
        assert_eq!(
            Direction { i: 1, j: 0 }.rotate_left().rotate_right(),
            Direction { i: 1, j: 0 }
        );
        assert_eq!(
            Direction { i: 0, j: -1 }.rotate_left().rotate_right(),
            Direction { i: 0, j: -1 }
        );
        assert_eq!(
            Direction { i: -1, j: 0 }.rotate_left().rotate_right(),
            Direction { i: -1, j: 0 }
        );
    }

    #[test]
    fn count_visited_positions_and_looping_obstructions_works_correctly() {
        let map = vec![
            "....#.....",
            ".........#",
            "..........",
            "..#.......",
            ".......#..",
            "..........",
            ".#..^.....",
            "........#.",
            "#.........",
            "......#...",
        ]
        .iter()
        .map(|line| Vec::from(line.as_bytes()))
        .collect();
        assert_eq!(
            count_visited_positions_and_looping_obstructions(map),
            (41, 6)
        );
    }
}

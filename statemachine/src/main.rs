use crate::States::*;
#[allow(dead_code)]
#[derive(Eq, PartialEq, Debug)]
enum States {
    S,
    S1,
    S11,
    S114,
    S1145,
    S11451,
    S114514,
}
fn next_state(cur: (States, i32)) -> States {
    match cur {
        (S, 1) => S1,
        (S1, 1) => S11,
        (S11, 1) => S11,
        (S11, 4) => S114,
        (S114, 1) => S1,
        (S114, 5) => S1145,
        (S1145, 1) => S11451,
        (S11451, 1) => S11,
        (S11451, 4) => S114514,
        (S114514, 1) => S1,
        _ => S,
    }
}
fn check(sequence: Vec<i32>) -> Vec<i32> {
    let (mut status, goal) = (S, S114514);
    let mut ans: Vec<i32> = Vec::new();
    for (i, item) in sequence.iter().enumerate() {
        status = next_state((status, *item));
        if status == goal {
            ans.push((i - 5) as i32);
        }
    }
    ans
}
fn main() {
    let source = String::from("11451411451114514");
    let sequence: Vec<i32> = source.split("").filter_map(|s| s.parse().ok()).collect();
    let ans = check(sequence);
    println!("{:?}", ans);
}

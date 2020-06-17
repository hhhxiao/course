// map f [a1; ...; an] = [f a1; ..., f an]
// An implementation alternative to the library function List.map
let rec myMap (f: 'a -> 'b) (lst: 'a list) : 'b list =
  match lst with
    | []     -> []
    | a::rst -> f a :: myMap f rst

// fold f [a1; ...; an] = f ... (f (f acc a1) a2) ...) an
// Exercise: replace the library function here with a recursive implementation for fold!
let rec myFold (f: 'b -> 'a -> 'b) (acc: 'b) (lst: 'a list) : 'b =
    match lst with
    | []     -> acc
    | a::rst -> myFold f (f acc a) rst
// filter f [a1; ...; an] = [ai; aj; ak ...], where f ai = true etc.,
// Exercise: replace the library function here with a recursive implementation for filter!
let rec myFilter (p: 'a -> bool) (lst: 'a list) : 'a list =
  match lst with
  |[] -> []
  |a::rst ->
    match (p a) with
      | true -> a::(myFilter p rst) 
      | false -> myFilter p rst
// Generate a list of n random positive integers less than 10
let genRandomNumbers n =
  let rnd = System.Random()
  List.init n (fun _ -> rnd.Next (10))

let printErrorMessage () =
  printfn "Program Input should be either 'map n', 'fold n', or 'filter n', where 'n' is a positive integer"

[<EntryPoint>]
let main (paramList : string []) : int =
  if paramList.Length <> 2 then
    printErrorMessage ()
    0
  else
    let n = int paramList.[1]
    if n < 0 then
      printErrorMessage ()
      0
    else
      let lst = genRandomNumbers n
      printfn "Random list is: %A" lst
      let (is_ok, res) = 
        match paramList.[0] with
          | "map" ->
            let map_fun x = 2 * x   // local function declaration
            let map_res = myMap map_fun lst
            let cor_res = List.map map_fun lst
            (map_res = cor_res, map_res)
            // printfn "Result of 'map (*2) %A': %A\n" lst map_res
          | "fold" ->
            let fold_fun = fun x y -> 2 * y :: x // lambda declaration
            let fld_res = myFold fold_fun [] lst
            let cor_res = List.fold fold_fun [] lst
            (fld_res = cor_res, fld_res)

          | "filter" ->
            let flt_res = myFilter (fun x -> x > 4) lst  // inlined lambda
            let cor_res = List.filter (fun x -> x > 4) lst
            (flt_res = cor_res, flt_res)
                     
          | _ ->
            printErrorMessage ()
            (false,[])
      if is_ok then
        printfn "Result is CORRECT  : %A" res
      else
        printfn "Result is INCORRECT: %A" res
      1

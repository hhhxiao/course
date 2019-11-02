data = load("./data/Iris-test.txt");
load('W1.mat','W1');
load('W2.mat','W2');
function d = format(data)
 d_max = max(max(data))
 d_min = max(min(data))
 data = data/(d_max-d_min)
end
a =rand(1,10)
format(a)

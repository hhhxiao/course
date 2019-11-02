
data = load("./data/Iris-train.txt");
test_data = load("./data/Iris-test.txt");

function d = get_d(i)
    d = zeros(3,1);
    d(i+1) = 1;
end

function d = format(data)
 d_max = max(max(data));
 d_min = max(min(data));
 d = data-d_min*ones(size(data))./(d_max-d_min);
end

%input(x)(4*1) -- W1(Wih)(10*4) 
 % --- hide(10*1) -- W2(3*10)  
 % - output(3*1)

function result = f(s)
    result = 1./(1 + exp(-1*s));
end
accu = ones(1,10);
for time = 1:10
    W1 = rand(10,4);
    W2 = rand(3,10);
    theta_1 = rand(10,1) ;
    theta_2 = rand(3,1) ;
    eta = 0.3;
    batch = 300;
    r = randperm(75 * batch);
    for j = 1:75*batch;
        i = mod(r(j),75)+1;
        sample = data(i,:);
        input_x = format(sample(1:4))';
        d =  get_d(sample(5));
        %   10*4  X  4*1;
        %隐含层节点值
        a_x = W1*input_x - theta_1;
        %激活后的隐含层节点值
        b = f(a_x);%10*1
        %    3*10 X 10*1
        %输出层节点值
        y_x = W2 * b - theta_2;
        %激活后的输出层节点值
        y_y = f(y_x); %(3*1)
        delta_output = d - y_y;
        %中间变量g (3*1)
        g = y_y .* (ones(3,1)-y_y).*delta_output;
        %权值矩阵2的更新值   3*1 X 10*1
        dw2 = eta * g * b';
        %阈值2的更新值
        %3*1       3*1
        d_theta_2 = -1*eta*g;

        %更新第二层
        %10*1  X (3*10 X 1*3)
        e = b.*(ones(10,1)-b).*(W2' * g);
        %        10*1 X 4*1
        dw1 = eta * e * input_x';
        d_theta_1 = -1*eta * e;

        theta_1 = theta_1 + d_theta_1;
        theta_2 = theta_2 + d_theta_2;
        W1 = W1 + dw1;
        W2 = W2 + dw2;
    end
    %train
    num = 0;
    for test_num = 1:75
        train_data = test_data(test_num,:);
        in = format(train_data(1:4))';
        d =  train_data(5);
        s_h = W1 * in;  %(10 * 1)
        y_h = f(s_h); %(10 * 1)
        s_j = W2 * y_h;  %(3 * 1)
        y_j = f(s_j); %(3 * 1)
        %disp(y_j)
        [t,index] = max(y_j);
        if(index == d+1)
            num = num+1;    
        end
    end
accu(time) = num/75;
num/75
end
avg = sum(accu)/10

f = sum((ones(1,10)*avg - accu).^2)/10

function res = su_newton(hint)
%How Sir Isaac Newton would solve a Sudoku
%Works only for sudokus with few empty fields or very good initial values
pos = find(hint==0);

%Arbitrary starting values
val = 9*rand(1,length(pos));

if(0)
	%Nelder-Mead
	val = fminsearch(@(x) norm(su_derive(hint,pos,x),2),val);
else
	%Newton

	
	df = su_derive(hint,pos,val);
	
	while(max(abs(df)) > 1e-30 && max(abs(df)) < 1e30)
		val = val - df/6;
		df = su_derive(hint,pos,val);
		max(abs(df))
	end
end

hint(pos) = val;

res = round(hint);

return


function res = su_derive(a,pos,x)
%derives the sudoku a(pos) = x

bs_index = [1 1 1 2 2 2 3 3 3; ...
			1 1 1 2 2 2 3 3 3; ...
			1 1 1 2 2 2 3 3 3; ...
			4 4 4 5 5 5 6 6 6; ...
			4 4 4 5 5 5 6 6 6; ...
			4 4 4 5 5 5 6 6 6; ...
			7 7 7 8 8 8 9 9 9; ...
			7 7 7 8 8 8 9 9 9; ...
			7 7 7 8 8 8 9 9 9];

a(pos) = x;

for i = 1:9
	rowsum(i) = sum(a(i,:));
	colsum(i) = sum(a(:,i));
	blsum(i) = sum(sum(a.*(bs_index == i)));
end

res = zeros(size(pos));

for i = 1:length(x)
	[row,col] = ind2sub([9,9],pos(i));
	res(i) = 2*(rowsum(row) + colsum(col) + blsum(bs_index(pos(i)))) - 270;
end
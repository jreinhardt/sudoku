function res = mc_sudoku(hint,T)

field = hint;

if size(hint) == [9 9]

	% for 9x9 sudoku
	bidx = [1 1 1 4 4 4 7 7 7; ...
			1 1 1 4 4 4 7 7 7; ...
			1 1 1 4 4 4 7 7 7; ...
			28 28 28 31 31 31 34 34 34; ...
			28 28 28 31 31 31 34 34 34; ...
			28 28 28 31 31 31 34 34 34; ...
			55 55 55 58 58 58 61 61 61; ...
			55 55 55 58 58 58 61 61 61; ...
			55 55 55 58 58 58 61 61 61];
	
	I_block = [0 1 2 9 10 11 18 19 20];
	
	su_size = 9;

elseif size(hint) == [4 4]

	bidx = [1 1 2 2; ...
			1 1 2 2; ...
			3 3 4 4; ...
			3 3 4 4];
	
	I_block = [0 1 4 5];

	su_size = 4;

else

	disp Bad Dimensions!

end

I_movable = find(field == 0);
num_movable = length(I_movable)

% fill the field with missing numbers
for i = 1:su_size
	I_free = find(field == 0);
	missing = su_size - length(find(field == i));
	field(I_free(1:missing)) = i;
end

i = 1;

while su_energy(field,su_size) > 0

	%choose 2 indices

	idx1 = I_movable(fix(rand()*num_movable) + 1);
	idx2 = I_movable(fix(rand()*num_movable) + 1);

	%  	%calc energy before
	score1 = su_energy(field,su_size)

	%swap
	field([idx1,idx2]) = field([idx2,idx1]);

	%calc energie after
	score2 = su_energy(field,su_size);

	%accept or reject swap
	if(score2 < score1)
		%accept
		hist(i) = score2;
	else
		diff = score2 - score1;
		if(exp(-diff/T) > rand)
			%accept
			hist(i) = score2;
		else
			%reject
			field([idx1,idx2]) = field([idx2,idx1]);
			hist(i) = score1;
		end
	end
	i = i + 1;
end

if su_energy(field,su_size) == 0
	disp Gelöst
else
	disp abgebrochen
	su_energy(field,su_size)
end

plot(hist)

i


res = field

return

function res = su_energy(field, su_size)

if su_size == 9

	% for 9x9 sudoku
	bidx = [1 4 7 28 31 34 55 58 61];
	
	I_block = [0 1 2 9 10 11 18 19 20];

	su_sum = 45;

elseif su_size == 4

	bidx = [1 3 9 11];
	
	I_block = [0 1 4 5];

	su_sum = 10;

else
	disp Bad dimensions!

end

res = 0;

for i = 1:su_size
	for j = 1:su_size
	res = res + norm(length(find(field(:,i) == j))-1) + norm(length(find(field(i,:) == j)) -1) + norm(length(find(field(bidx(i)+I_block) == j)) -1);
	end
end
function res = sudoku(hints, bsx, bsy)
% Processes the hints matrix to solve the sudoku
% hints shall contain indices > 0 for known symbols
% and 0 for unkown symbols
% bsx and bsy are the sizes of the blocks horizontal and vertical respectively

if size(hints,1) ~= size(hints,2)
	disp Fehlerhafte Dimension
end

su_size = size(hints,1);

bl_size = fix(sqrt(su_size));

if bl_size^2 ~= su_size
	disp Fehlerhafte Dimension
end

%TODO: generate bidx generically
bidx = [1 1 1 4 4 4 7 7 7];

rc_idx = [2 3; 1 3; 1 2];

field = true(su_size,su_size,su_size);

%read in hints
for i = 1:su_size
	for j = 1:su_size
		if hints(i,j) ~= 0
			field = su_set(field,i,j,hints(i,j));
		end
	end
end

finished = 0;

while finished < 100
	
	for i = 1:su_size
		for j = 1:su_size
			%last possible number in this field
			if sum(field(i,j,:)) == 1
				field = su_set(field,i,j,find(field(i,j,:) == 1));
			end
	
			% last possible position for number in row
			if sum(field(:,i,j)) == 1
				field = su_set(field,find(field(:,i,j) == 1),i,j);
			end
	
			% last possible position for number in column
			if sum(field(i,:,j)) == 1
				field = su_set(field,i,find(field(i,:,j) == 1),j);
			end

			% last possible position for number in block
			block = field(:,:,j);
			mask  = zeros(size(block));
			mask(bidx(ceil(i/3)):bidx(ceil(i/3)) + bl_size - 1,bidx(mod(i,3) + 1):bidx(mod(i,3) + 1) + bl_size - 1) = 1;
			block = block & mask;
			if sum(sum(block)) == 1
				[x y] = ind2sub(size(block),find(block == 1));
				field = su_set(field,x,y,j);
			end

			% elimination for all possibilities for a number in one row
			if 


	
		end

	end

	finished = finished + 1;
end

%checks:


% elimination for all possibilities for a number in one column

for i = 1:su_size
	for j = 1:su_size
res(i,j) = find(field(i,j,:) == 1);
	end
end

return

function fld = su_set(fld,i,j,val)
	
	if size(fld,1) ~= size(fld,2)
		disp Fehlerhafte Dimension
	end
	
	su_size = size(fld,1);
	
	bl_size = fix(sqrt(su_size));
	%TODO: generate bidx generically
	bidx = [1 1 1 4 4 4 7 7 7];

	fld(i,j,:) = 0;
	fld(i,:,val) = 0;
	fld(:,j,val) = 0;
	fld(bidx(i):bidx(i) + bl_size - 1,bidx(j):bidx(j) + bl_size - 1,val) = 0;
	fld(i,j,val) = 1;

return
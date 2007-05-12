num_hints = 9;

for i = 1:num_hints
	hint{i} = load(sprintf('hint%d',i));
end

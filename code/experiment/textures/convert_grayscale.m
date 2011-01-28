function [ converted_img ] = convert_grayscale( img )


imgSize = size(img);

converted_img = img;

%for i = 1: imgSize(1)
%    for j = 1: imgSize(2)
%        if img(i, j) >= 127
%            convirted_img(i, j) = img(i, j) * 0.5 + 127;
%        else
%            convirted_img(i, j) = (127 + img(i, j)) * 0.5;
%        end;
%    end;
%end;

for i = 1: imgSize(1)
    for j = 1: imgSize(2)
        converted_img(i, j) = (127 + img(i, j)) * 0.5;
    end;
end;


end


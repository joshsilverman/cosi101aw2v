require 'pry'

contents = ""
open('data/training_set.tsv', 'r+') do |f|
  contents = f.read
end

contents.gsub! '"', ''
contents.gsub! ',', ''
contents.gsub! "\t", ','

# stopword filter
common = {}
%w{ a and or to the is in be their for of }.each{|w| common[w] = true}
contents = contents.gsub(/\b\w+\b/){|word| common[word] ? '': word}.squeeze(' ')

contents = contents.downcase

open('data/training_set.csv', 'w') do |f|
  f.write contents
end
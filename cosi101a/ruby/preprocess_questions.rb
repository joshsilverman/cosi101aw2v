require 'pry'

['data/training_set', 'data/validation_set'].each do |filename|
  contents = ""
  open(filename + '.tsv', 'r+') do |f|
    contents = f.read
  end

  contents.gsub! '"', ''
  contents.gsub! ',', ''
  contents.gsub! "\t", ','
  contents.gsub! '.', ''

  # stopword filter
  common = {}
  %w{ a and or to the is in be their for of with an there it that as when are does in from by at which have its this }.each{|w| common[w] = true}
  contents = contents.gsub(/\b\w+\b/){|word| common[word] ? '': word}.squeeze(' ')

  contents = contents.downcase

  open(filename + '.csv', 'w') do |f|
    f.write contents
  end
end
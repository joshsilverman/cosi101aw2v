require 'csv'
#require 'pry'

ans_r = ["a", "b", "c", "d"]
cmd = "ConsoleApplication3.exe"
ret = `#{cmd}`
predictions = {}

qs_raw = ret.split("\n")[2..-1]
# binding.pry
qs_raw.each do |q|
  q_raw = q.split(', ')
  id = q_raw.shift.to_i
  ans = q_raw.map(&:to_f)
  prediction = ans.index(ans.max)
  if ans_r[prediction].nil?
	predictions[id]='a'
  else
	predictions[id] = ans_r[prediction]
  end
end

truth_set = {}
CSV.foreach("E:/GitDir/Similarity/cosi101a/data/training_set.csv") do |row|
  id = row[0].to_i
  truth_set[id] = row[2] if id > 0
  # binding.pry
end

correct_h = {}
correct_count = 0.0
total = 0.0
truth_set.each do |id, correct_letter|
  correct_bool = correct_letter.downcase == predictions[id].downcase
  correct_h[id] = correct_bool

  total += 1
  correct_count +=1 if correct_bool
end

# binding.pry
accuracy = correct_count / total
puts "correct percentage: #{accuracy}"


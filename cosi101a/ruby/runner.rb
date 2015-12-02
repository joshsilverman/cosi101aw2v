require 'csv'
require 'pry'

ans_r = ["A", "B", "C", "D"]

CSV.open("../data/submission.csv", "wb") do |csv|
  cmd = "./../distance"
  puts "cmd: #{cmd}"
  ret = `#{cmd}`
  
  csv << ['id', 'correctAnswer']
  qs_raw = ret.split("\n")[2..-1]
  qs_raw.each do |q|
    q_raw = q.split(', ')
    id = q_raw.shift
    ans = q_raw.map(&:to_f)
    prediction = ans.index(ans.max)
    # binding.pry

    puts "#{id}, #{ans_r[prediction]}"
    csv << [id, ans_r[prediction]]
  end
end

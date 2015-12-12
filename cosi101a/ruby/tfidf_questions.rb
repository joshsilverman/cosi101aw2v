require 'csv'
require 'pry'
require 'matrix'
require 'tf-idf-similarity'
require 'benchmark'
require 'narray'

# ['data/training_set', 'data/validation_set'].each do |filename|
sets = {'data/training_set' => [1,3,4,5,6] }#, 
        # 'data/validation_set' => [1,2,3,4,5] }

model = nil
puts "compiling documents..."
sets.each do |filename, indices|
  contents = ""
  open(filename + '.tsv', 'r+') do |f|
    contents = f.read
  end

  corpus = []
  CSV.foreach("#{filename}.csv") do |row|
    indices.each do |i|
      corpus << TfIdfSimilarity::Document.new(row[i])
    end
  end

  # corpus = corpus[0..100]
  puts "begining model construction..."
  t = Benchmark.measure { 
    model = TfIdfSimilarity::TfIdfModel.new(corpus) 
    # model = TfIdfSimilarity::BM25Model.new(corpus)
    # model = TfIdfSimilarity::TfIdfModel.new(corpus, :library => :narray) 
  }
  puts "completed in #{t.real} seconds"
  puts "begining term scoring..."

  t = Benchmark.measure do
    CSV.open("#{filename}_tfidf.csv", "wb") do |csv|    
      CSV.foreach("#{filename}.csv") do |row|
        indices.each do |i|
          document = TfIdfSimilarity::Document.new(row[i])
          tfidf_by_term = {}
          document.terms.each do |term|
            tfidf_by_term[term] = model.tfidf(document, term)
          end

          if tfidf_by_term.values.count == 0
            next
          end

          terms = []
          max = tfidf_by_term.values.max
          # min = tfidf_by_term.values.min
          # range = max - min
          # threshold = range / 4.0 + min

          # average = tfidf_by_term.reduce(:+) / tfidf_by_term.size.to_f
          threshold = [5.5, max].min

          tfidf_by_term.each do |term, score|
            if score >= threshold 
              terms << term
            end
          end

          row[i] = terms.join " "
        end
        csv << row
      end
    end
  end
  puts "completed in #{t.real} seconds"
end
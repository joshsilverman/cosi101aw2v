require 'csv'
require 'pry'
require 'matrix'
require 'tf-idf-similarity'
# require 'narray'

# ['data/training_set', 'data/validation_set'].each do |filename|
sets = {'data/training_set' => [1,3,4,5,6] }#, 
        # 'data/validation_set' => [1,2,3,4,5] }

model = nil
sets.each do |filename, indices|
  contents = ""
  open(filename + '.tsv', 'r+') do |f|
    contents = f.read
  end

  corpus = []
  CSV.foreach("#{filename}.csv") do |row|
    indices.each do |i|
      corpus << TfIdfSimilarity::Document.new(row[i])
      # begin
      # rescue
      #   binding.pry
      # end
    end
  end

  # corpus = corpus[0..100]
  binding.pry
  model = TfIdfSimilarity::TfIdfModel.new(corpus)
  binding.pry

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
        max_score = tfidf_by_term.values.max
        average_score = tfidf_by_term.reduce(:+) / tfidf_by_term.size.to_f
        threshold = [average_score, max_score].min
        # begin
        # rescue
        #   binding.pry
        # end
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
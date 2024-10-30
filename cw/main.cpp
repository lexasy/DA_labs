#include <bits/stdc++.h>

// Foo for splitting documents on words by delimeters "!", "?", ".", "," and " "; "!", "?", "." theese delimeters needs, because sentence could ending with theese symbols
// references to: https://ru.stackoverflow.com/questions/1469554/%D0%A0%D0%B0%D0%B7%D0%B1%D0%B8%D1%82%D1%8C-%D1%81%D1%82%D1%80%D0%BE%D0%BA%D1%83-%D0%BD%D0%B0-%D1%81%D0%BB%D0%BE%D0%B2%D0%B0-%D0%A1
std::vector<std::string> WordsSplit(std::string sentence)
{
    std::stringstream ss(sentence);
    std::vector<std::string> words;
    const char* const delimeters = "!?., ";
    std::string line;
    while (std::getline(ss, line))
    {
        char *token = std::strtok(line.data(), delimeters);
        while (token != nullptr)
        {
            words.push_back(token);
            token = std::strtok(nullptr, delimeters);
        }
    }
    return words;
}

// Foo for turning words in low registry, this needs becase in our paradigma Cat and cat are the same words
std::string ToLower(std::string input)
{
    for (size_t i = 0; i < input.size(); ++i)
    {
        input[i] = std::tolower(static_cast<unsigned char>(input[i]));
    }
    return input;
}

class TWordStats
{
public:
    // Default constructor
    TWordStats() = default;
    // stat fields
    std::vector<int> frequencies = {0, 0}; //[0] - zero class, [1] - first class
};

// argmax p(C = c) * П p(F_i = f_i | C = c)
// C - classes
// F_i - words
class TNaiveBayes
{
// 0 - zero class, 1 - first class
private:
    std::unordered_map<std::string, TWordStats> word_stats; //{word: [frequency in zero class, frequency in first class], ...}
    // prior probability of zero class, we don't need prior probability of first class, because this calculates as 1 - prior probability of zero class
    long double prior_probability_zero;
    // method for summaring frequencies of each class, this needs for calculating prior probability of classes
    std::pair<int, int> sum_freqs()
    {
        int zero_count = 0;
        int first_count = 0;
        for (const auto& pair : this->word_stats)
        {
            zero_count += pair.second.frequencies[0];
            first_count += pair.second.frequencies[1];
        }
        return {zero_count, first_count};
    }
    // Method for calculating result probability of class for sentence
    long double probability_calculator(std::vector<std::string>& words, int class_id)
    {
        long double probability = 1;
        for (size_t i = 0; i < words.size(); ++i)
        {
            if (!this->word_stats[words[i]].frequencies[class_id])
            {
                // if we meet the word, that doesn't exist in train_sample or this word doesn't exist in specified class, we can't take probability of this word as 0
                // because we get result probability 0. So, if this probability 0, in theory, on practice we can take very small number, for example epsilon, for probability of this word
                probability *= 1e-8;
            }
            else
            {
                probability *= 1.0 * this->word_stats[words[i]].frequencies[class_id] / (this->word_stats[words[i]].frequencies[0] + this->word_stats[words[i]].frequencies[1]);
            }
            
        }
        probability *= 1.0 * (class_id == 1 ? 1 - this->prior_probability_zero : this->prior_probability_zero);
        return probability;
    }
public:
    // Default constructor
    TNaiveBayes() = default;
    // fit method
    void fit(std::vector<std::string>& X, std::vector<int>& y)
    {
        for (size_t i = 0; i < X.size(); ++i)
        {
            std::vector<std::string> words = WordsSplit(ToLower(X[i]));
            for (size_t j = 0; j < words.size(); ++j)
            {
                // if we already have the word in dict, we just incrementing its frequency for specify class
                if (this->word_stats.count(words[j]))
                {
                    word_stats[words[j]].frequencies[y[i]]++;
                }
                // else, we add new word in our dict
                else
                {
                    TWordStats stats;
                    stats.frequencies[y[i]]++;
                    word_stats[words[j]] = stats;
                }
            }
        }
        // now we can calculate prior probability of zero class
        std::pair freqs = this->sum_freqs();
        this->prior_probability_zero = 1.0 * freqs.first / (freqs.first + freqs.second);
    }

    // predict method for one sentence
    int predict(std::string X)
    {
        std::vector<std::string> words = WordsSplit(ToLower(X));
        // calculating probabities of each class for this sentence
        long double probability_zero = this->probability_calculator(words, 0);
        long double probability_first = this->probability_calculator(words, 1);
        // take argmax
        return probability_zero > probability_first ? 0 : 1;
    }
};

int main()
{
    int train_files_count, test_files_count;
    std::vector<std::string> train_sample;
    std::vector<int> target;

    // reading data
    std::cin >> train_files_count >> test_files_count;
    for (int _ = 0; _ < train_files_count; _++)
    {
        int class_id;
        std::string train_file_text;
        std::cin >> class_id;
        target.push_back(class_id);
        std::cin.ignore();
        getline(std::cin, train_file_text);
        train_sample.push_back(train_file_text);
    }
    TNaiveBayes classificator;
    classificator.fit(train_sample, target);
    for (int _ = 0; _ < test_files_count; _++)
    {
        std::string test_file_text;
        getline(std::cin, test_file_text);
        std::cout << classificator.predict(test_file_text) << "\n";
    }
}
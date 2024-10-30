#include <bits/stdc++.h>

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
    int frequency_in_zero = 0;
    int frequency_in_first = 0;
    double probability_in_zero, probability_in_first;
    // filling probabilities method
    void filling_probabilities(int total_amount_in_zero, int total_amount_in_first)
    {
        this->probability_in_zero = double(this->frequency_in_zero + 1) / double(total_amount_in_zero);
        this->probability_in_first = double(this->frequency_in_first + 1) / double(total_amount_in_first);
    }
};

class TNaiveBayes
{
// 0 - zero class, 1 - first class
public:
    std::unordered_map<std::string, TWordStats> word_stats;
    double prior_probability_zero;
private:
    std::pair<int, int> sum_freqs()
    {
        int zero_count = 0;
        int first_count = 0;
        for (const auto& pair : this->word_stats)
        {
            zero_count += pair.second.frequency_in_zero;
            first_count += pair.second.frequency_in_first;
        }
        return {zero_count, first_count};
    }
    std::pair<double, double> probability_calculator(std::vector<std::string> words)
    {
        double probability_zero = this->prior_probability_zero;
        double probability_first = 1 - this->prior_probability_zero;
        for (size_t i = 0; i < words.size(); ++i)
        {
            probability_zero *= this->word_stats[words[i]].probability_in_zero;
            probability_first *= this->word_stats[words[i]].probability_in_first;
        }
        return {probability_zero, probability_first};
    }
public:
    // Default constructor
    TNaiveBayes() = default;
    // fit method
    void fit(std::vector<std::string> X, std::vector<int> y)
    {
        this->prior_probability_zero = float(std::count(std::cbegin(y), std::cend(y), 0)) / y.size();
        for (size_t i = 0; i < X.size(); ++i)
        {
            std::vector<std::string> words = WordsSplit(ToLower(X[i]));
            for (size_t j = 0; j < words.size(); ++j)
            {
                if (this->word_stats.count(words[j]))
                {
                    if (y[i] == 1)
                    {
                        word_stats[words[j]].frequency_in_first++;
                    }
                    else
                    {
                        word_stats[words[j]].frequency_in_zero++;
                    }
                }
                else
                {
                    TWordStats stats;
                    if (y[i] == 1)
                    {
                        stats.frequency_in_first++;
                    }
                    else
                    {
                        stats.frequency_in_zero++;
                    }
                    word_stats[words[j]] = stats;
                }
            }
        }
    }

    int predict(std::string X)
    {
        int result;
        std::vector<std::string> words = WordsSplit(ToLower(X));
        std::vector<std::string> words_to_delete;
        for (size_t j = 0; j < words.size(); ++j)
        {
            if (!this->word_stats.count(words[j]))
            {
                words_to_delete.push_back(words[j]);
                TWordStats stats;
                word_stats[words[j]] = stats;
            }
        }
        std::pair<int, int> freqs_sum = this->sum_freqs();
        for (auto& pair : this->word_stats)
        {
            pair.second.filling_probabilities(freqs_sum.first + this->word_stats.size(), freqs_sum.second + this->word_stats.size());
            // std::cout << pair.first << ": " << pair.second.frequency_in_zero + 1 << " " << pair.second.frequency_in_first + 1 << " " << pair.second.probability_in_zero << " " << pair.second.probability_in_first << "\n";
        }   
        std::pair<float, float> probabilities = this->probability_calculator(words);
        result = (probabilities.first > probabilities.second ? 0 : 1);
        for (size_t j = 0; j < words_to_delete.size(); ++j)
        {
            this->word_stats.erase(words_to_delete[j]);
        }
        return result;
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
    // for (const auto& pair : classificator.word_stats)
    // {
    //     std::cout << pair.first << " " << pair.second.frequency_in_zero << " " << pair.second.frequency_in_first << "\n";
    // }
    // std::cout << "\n";
}
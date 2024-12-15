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
    std::vector<int> frequencies = {0, 0}; // [0] - zero class, [1] - first class
};

class TNaiveBayes
{
private:
    std::unordered_map<std::string, TWordStats> word_stats;
    long double prior_probability_zero;
    std::size_t dict_size;
    std::vector<int> freqs;

    std::vector<int> sum_freqs()
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

    long double probability_calculator(std::vector<std::string>& words, int class_id)
    {
        long double probability = 1;
        for (size_t i = 0; i < words.size(); ++i)
        {
            if (!this->word_stats[words[i]].frequencies[class_id])
            {
                probability *= 1.0 / (freqs[class_id] + this->dict_size);
            } else {
                probability *= 1.0 * (this->word_stats[words[i]].frequencies[class_id] + 1) / (this->freqs[class_id] + this->dict_size);
            }
        }
        probability *= 1.0 * (class_id == 1 ? 1 - this->prior_probability_zero : this->prior_probability_zero);
        return probability;
    }

public:
    void fit(std::vector<std::string>& X, std::vector<int>& y)
    {
        this->prior_probability_zero = float(std::count(std::cbegin(y), std::cend(y), 0)) / y.size();
        for (size_t i = 0; i < X.size(); ++i)
        {
            std::vector<std::string> words = WordsSplit(ToLower(X[i]));
            for (size_t j = 0; j < words.size(); ++j)
            {
                if (this->word_stats.count(words[j]))
                {
                    word_stats[words[j]].frequencies[y[i]]++;
                } else
                {
                    TWordStats stats;
                    stats.frequencies[y[i]]++;
                    word_stats[words[j]] = stats;
                }
            }
        }
        this->dict_size = word_stats.size();
        this->freqs = this->sum_freqs();
    }

    int predict(std::string X)
    {
        std::vector<std::string> words = WordsSplit(ToLower(X));
        long double probability_zero = this->probability_calculator(words, 0);
        long double probability_first = this->probability_calculator(words, 1);
        return probability_zero > probability_first ? 0 : 1;
    }

    void save_stats(const std::string& filename)
    {
        std::ofstream ofs(filename);
        ofs << prior_probability_zero << "\n";
        ofs << dict_size << "\n";
        for (const auto& pair : word_stats)
        {
            ofs << pair.first << " " << pair.second.frequencies[0] << " " << pair.second.frequencies[1] << "\n";
        }
    }

    void load_stats(const std::string& filename)
    {
        std::ifstream ifs(filename);
        ifs >> prior_probability_zero;
        ifs >> dict_size;
        word_stats.clear();
        std::string word;
        int freq0, freq1;
        while (ifs >> word >> freq0 >> freq1)
        {
            TWordStats stats;
            stats.frequencies[0] = freq0;
            stats.frequencies[1] = freq1;
            word_stats[word] = stats;
        }
        freqs = sum_freqs();
    }
};

int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        std::cerr << "Usage:\n"
                  << "./a.out learn --input <input file> --output <stats file>\n"
                  << "./a.out classify --stats <stats file> --input <input file> --output <output file>\n";
        return 1;
    }

    std::string command = argv[1];
    TNaiveBayes classifier;

    if (command == "learn")
    {
        std::string input_file = argv[3];
        std::string output_file = argv[5];

        std::vector<std::string> train_sample;
        std::vector<int> target;

        std::ifstream infile(input_file);
        int class_id;
        std::string train_file_text;
        while (infile >> class_id)
        {
            target.push_back(class_id);
            infile.ignore();
            std::getline(infile, train_file_text);
            train_sample.push_back(train_file_text);
        }
        classifier.fit(train_sample, target);
        classifier.save_stats(output_file); // statistics saving
    } else if (command == "classify")
    {
        std::string stats_file = argv[3];
        std::string input_file = argv[5];
        std::string output_file = argv[7];

        classifier.load_stats(stats_file); // statistics loading

        std::ifstream infile(input_file);
        std::ofstream outfile(output_file);
        std::string test_file_text;

        while (std::getline(infile, test_file_text))
        {
            int prediction = classifier.predict(test_file_text);
            outfile << prediction << "\n";
        }
    } else
    {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }
}

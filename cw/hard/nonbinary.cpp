#include <bits/stdc++.h>

const double PREDICTION_EDGE = 0.1;

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

long double max_val_vec(std::vector<std::pair<std::string, long double>>& vec)
{
    long double max_value = std::numeric_limits<long double>::lowest();

    for (const auto& p : vec) {
        if (p.second > max_value) {
            max_value = p.second;
        }
    }
    return max_value;
}

std::vector<std::pair<std::string, long double>> softmax(std::vector<std::pair<std::string, long double>> vec)
{
    std::vector<std::pair<std::string, long double>> result;
    long double max_val = max_val_vec(vec);
    long double exp_sum = 0;
    std::vector<long double> exp_values;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        long double exp_val = std::exp(vec[i].second - max_val);
        exp_values.push_back(exp_val);
        exp_sum += exp_val;
    }

    for (size_t i = 0; i < vec.size(); ++i)
    {
        result.push_back({vec[i].first, exp_values[i] / exp_sum});
    }
    return result;
}

class TWordStats
{
public:
    std::unordered_map<std::string, int> frequencies; // frequencies of word for each class
};

class TNaiveBayes
{
private:
    std::unordered_map<std::string, TWordStats> word_stats;
    std::unordered_map<std::string, long double> prior_probabilities;
    std::size_t dict_size;
    std::unordered_map<std::string, int> freqs;
    std::vector<std::string> class_labels;
    int questions_quantity;
    std::unordered_map<std::string, int> tags_quantity;

    std::unordered_map<std::string, int> sum_freqs()
    {
        std::unordered_map<std::string, int> counts;
        for (auto& pair : this->word_stats)
        {
            for (const auto& class_label : class_labels)
            {
                counts[class_label] += pair.second.frequencies[class_label];
            }
        }
        return counts;
    }

    long double probability_calculator(std::vector<std::string>& words, const std::string& class_label)
    {
        long double probability = 0;
        for (const auto& word : words)
        {
            if (this->word_stats.count(word) == 0 || this->word_stats[word].frequencies[class_label] == 0)
            {
                // Laplas smoothing
                probability += log(1.0) - log(freqs[class_label] + dict_size);
            }
            else
            {
                probability += log(this->word_stats[word].frequencies[class_label] + 1) - log(freqs[class_label] + dict_size);
            }
        }
        probability += prior_probabilities[class_label];
        return probability;
    }

public:
    void fit(std::vector<std::string>& X, std::vector<std::vector<std::string>>& y)
    {
        questions_quantity = X.size();
        std::set<std::string> unique_classes;
        for (const auto& tags : y)
        {
            for (const auto& tag : tags)
            {
                unique_classes.insert(tag);
            }
        }
        class_labels.assign(unique_classes.begin(), unique_classes.end());

        for (const auto& class_label : class_labels)
        {
            prior_probabilities[class_label] = 0;
            freqs[class_label] = 0;
        }

        for (size_t i = 0; i < X.size(); ++i)
        {
            std::vector<std::string> words = WordsSplit(ToLower(X[i]));
            for (const auto& class_label : y[i])
            {
                for (const auto& word : words)
                {
                    if (this->word_stats.count(word))
                    {
                        word_stats[word].frequencies[class_label]++;
                    }
                    else
                    {
                        TWordStats stats;
                        stats.frequencies[class_label] = 1;
                        word_stats[word] = stats;
                    }
                    freqs[class_label]++;
                }
                prior_probabilities[class_label] += 1;
            }
        }
        for (auto& class_label : class_labels)
        {
            tags_quantity[class_label] = prior_probabilities[class_label];
            prior_probabilities[class_label] = log(prior_probabilities[class_label]) - log(X.size());
            // std::cout << class_label << ": " << prior_probabilities[class_label] << "\n";
        }
        // std::cout << "\n";
        this->dict_size = word_stats.size();
    }

    std::vector<std::string> predict(std::string X)
    {
        // std::cout << X << "\n";
        std::vector<std::string> words = WordsSplit(ToLower(X));
        std::vector<std::pair<std::string, long double>> prediction;
        for (const auto& class_label : class_labels)
        {
            long double probability = this->probability_calculator(words, class_label);
            prediction.push_back({class_label, probability});
        }
        prediction = softmax(prediction);
        std::sort(prediction.begin(), prediction.end(), [](const std::pair<std::string, long double>& a, const std::pair<std::string, long double>& b)
        {
            return a.second > b.second;
        });
        std::vector<std::string> result;
        for (size_t i = 0; i < prediction.size(); i++)
        {
            if (prediction[i].second > PREDICTION_EDGE)
            {
                result.push_back(prediction[i].first);
            }
        }
        return result;
    }

    void save_stats(const std::string& filename)
    {
        std::ofstream ofs(filename);
        // ofs << "=====QUESTIONS QUANTITY=====";
        ofs << questions_quantity << "\n";
        // ofs << "=====DICT SIZE=====\n";
        ofs << dict_size << "\n";
        // ofs << "=====UNIQUE TAGS QUANTITY=====\n";
        ofs << class_labels.size() << "\n";
        // ofs << "=====CLASSES FREQUENCY=====\n";
        for (const auto& class_label : class_labels)
        {
            ofs << class_label << " " << tags_quantity[class_label] << "\n";
        }
        // ofs << "=====WORD STATS=====\n";
        for (auto& pair : word_stats)
        {
            ofs << pair.first;
            for (const auto& class_label : class_labels)
            {
                ofs << " " << pair.second.frequencies[class_label];
            }
            ofs << "\n";
        }
    }

    void load_stats(const std::string& filename)
    {
        std::ifstream ifs(filename);
        // ifs.ignore();
        ifs >> questions_quantity;
        // ifs.ignore();
        ifs >> dict_size;
        class_labels.clear();
        prior_probabilities.clear();
        tags_quantity.clear();
        std::string class_label;
        // ifs.ignore();
        size_t tags;
        ifs >> tags;
        // ifs.ignore();
        for (size_t _ = 0; _ < tags; _++)
        {
            std::string class_label;
            ifs >> class_label;
            class_labels.push_back(class_label);
            ifs >> tags_quantity[class_label];
            prior_probabilities[class_label] = log(tags_quantity[class_label]) - log(questions_quantity);
            std::cout << class_label << ": " << prior_probabilities[class_label] << "\n";
        }
        // ifs.ignore();
        word_stats.clear();
        std::string word;
        while (ifs >> word)
        {
            TWordStats stats;
            for (const auto& classes : class_labels)
            {
                int freq;
                ifs >> freq;
                stats.frequencies[classes] = freq;
            }
            word_stats[word] = stats;
        }
        freqs = sum_freqs();
    }
};

int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        std::cerr << "./a.out learn --input <input file> --output <stats file>\n"
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
        std::vector<std::vector<std::string>> target;

        std::ifstream infile(input_file);

        while (infile)
        {
            int num_lines;
            infile >> num_lines;
            if (infile.eof())
            {
                break;
            }
            infile.ignore();

            std::string tags_line;
            std::getline(infile, tags_line);
            if (tags_line.empty())
            {
                break;
            }
            std::vector<std::string> tags = WordsSplit(tags_line);

            std::string question_title;
            std::getline(infile, question_title);

            std::string question_text;
            for (int i = 0; i < num_lines; ++i)
            {
                std::string line;
                std::getline(infile, line);
                question_text += line + "\n";
            }

            train_sample.push_back(question_title + "\n" + question_text);
            target.push_back(tags);
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

        while (infile)
        {
            if (infile.eof())
            {
                break;
            }
            int num_lines;
            infile >> num_lines;
            infile.ignore();

            std::string question_title;
            std::getline(infile, question_title);

            std::string question_text;
            for (int i = 0; i < num_lines; ++i)
            {
                std::string line;
                std::getline(infile, line);
                question_text += line;
            }
            std::vector<std::string> prediction = classifier.predict(question_title + "\n" + question_text);
            if (prediction.size() != 0)
            {
                outfile << prediction[0];
            }
            for (size_t i = 1; i < prediction.size(); ++i)
            {
                outfile << ", " << prediction[i];
            }
            outfile << "\n";
        }
    } else
    {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }
}

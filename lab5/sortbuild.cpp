#include <iostream>
#include <vector>
#include <string>

const size_t ALPHABET_SIZE = 256;

class TItem
{
public:
    int idx, old_equivalence_class, current_equivalence_class;
    TItem(int idx, int old_equvalence_class, int current_equivalence_class)
    {
        this->idx = idx;
        this->old_equivalence_class = old_equvalence_class;
        this->current_equivalence_class = current_equivalence_class;
    }
};

int modulo(int a, int b)
{
    return (a >= 0 ? a % b : (b + a) % b);
}

std::vector<int> CountingSortWithIdxs(std::vector<char>& array)
{
    int tmp[ALPHABET_SIZE] = {0};
    std::vector<char> result = std::vector<char>(array.size(), char(0));
    std::vector<int> result_idxs = std::vector<int>(array.size(), 0);
    for (size_t i = 0; i < array.size(); ++i)
    {
        tmp[int(array[i])]++;
    }
    for (size_t i = 1; i < ALPHABET_SIZE; ++i)
    {
        tmp[i] += tmp[i - 1];
    }
    for (size_t i = array.size(); i > 0; --i)
    {
        --tmp[int(array[i - 1])];
        result[tmp[int(array[i - 1])]] = array[i - 1];
        result_idxs[tmp[int(array[i - 1])]] = i - 1;
    }
    for (size_t i = 0; i < array.size(); ++i)
    {
        array[i] = result[i];
    }
    return result_idxs;
}

void CountingSort(std::vector<TItem>& array)
{
    int tmp[ALPHABET_SIZE] = {0};
    std::vector<TItem> result = std::vector<TItem>(array.size(), {0, 0, 0});
    for (size_t i = 0; i < array.size(); ++i)
    {
        result[i].old_equivalence_class = array[i].old_equivalence_class;
    }
    for (size_t i = 0; i < array.size(); ++i)
    {
        tmp[array[i].current_equivalence_class]++;
    }
    for (size_t i = 1; i < ALPHABET_SIZE; ++i)
    {
        tmp[i] += tmp[i - 1];
    }
    for (size_t i = array.size(); i > 0; --i)
    {
        --tmp[array[i - 1].current_equivalence_class];
        result[tmp[array[i - 1].current_equivalence_class]].current_equivalence_class = array[i - 1].current_equivalence_class;
        result[tmp[array[i - 1].current_equivalence_class]].idx = array[i - 1].idx;
    }
    for (size_t i = 0; i < array.size(); ++i)
    {
        array[i] = result[i];
    }
}

std::vector<int> SuffixArrayBuilder(std::string text, int added_sentinels)
{
    std::vector<char> text_vector;
    std::vector<int> equivalence_classes = std::vector<int>(text.size(), 0);
    for (size_t i = 0; i < text.size(); ++i)
    {
        text_vector.push_back(text[i]);
    }
    std::vector<int> suffix_array = CountingSortWithIdxs(text_vector);
    equivalence_classes[0] = 0;
    for (size_t i = 1; i < text.size(); ++i)
    {
        equivalence_classes[i] = (text_vector[i] == text_vector[i - 1] ? equivalence_classes[i - 1] : equivalence_classes[i - 1] + 1);
    }
    std::vector<int> equivalence_classes_specified_index = std::vector<int>(text.size(), 0);
    for (size_t i = 0; i < suffix_array.size(); ++i)
    {
        equivalence_classes_specified_index[suffix_array[i]] = equivalence_classes[i];
    }
    std::vector<TItem> items_array;
    for (size_t i = 0; i < text.size(); ++i)
    {
        items_array.push_back({suffix_array[i], equivalence_classes_specified_index[i], equivalence_classes_specified_index[modulo((suffix_array[i] - 1), text.size())]});
    }
    for (int p = 0; (1 << p) < text.size(); ++p)
    {
        for (size_t i = 0; i < text.size(); ++i)
        {
            items_array[i].idx = modulo((items_array[i].idx - (1 << p)), text.size());
            items_array[i].current_equivalence_class = items_array[items_array[i].idx].old_equivalence_class;
        }
        CountingSort(items_array);
        std::vector<std::pair<int, int>> pair_equivalence_classes;
        for (size_t i = 0; i < items_array.size(); ++i)
        {
            pair_equivalence_classes.push_back({items_array[i].current_equivalence_class, items_array[modulo((items_array[i].idx + (1 << p)), text.size())].old_equivalence_class});
        }
        items_array[0].current_equivalence_class = 0;
        for (size_t i = 1; i < items_array.size(); ++i)
        {
            items_array[i].current_equivalence_class = (pair_equivalence_classes[i] == pair_equivalence_classes[i - 1] ? items_array[i - 1].current_equivalence_class : items_array[i - 1].current_equivalence_class + 1);
            items_array[items_array[i].idx].old_equivalence_class = items_array[i].current_equivalence_class;
        }
    }
    std::vector<int> result_suffix_array;
    for (size_t i = added_sentinels; i < items_array.size(); ++i)
    {
        result_suffix_array.push_back(items_array[i].idx);
    }
    return result_suffix_array;
}

int NextPowerOf2(size_t size)
{
    int power = 0;
    while (size > (1 << power))
    {
        power++;
    }
    return (1 << power);
}


int main()
{
    std::string input_text;
    std::cin >> input_text;
    int added_sentinels = NextPowerOf2(input_text.size() + 1) - input_text.size(); // +1 - $ в конце, -1 $ в конце, +1 не учитывать $ в конце
    input_text += std::string(added_sentinels, '$');
    std::vector<int> suffix_array = SuffixArrayBuilder(input_text, added_sentinels);
    for (size_t i = 0; i < suffix_array.size(); ++i)
    {
        std::cout << suffix_array[i] << " ";
    }
    std::cout << "\n";
}
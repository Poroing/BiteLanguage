#include "stream.hpp"
#include <iterator>
#include <exception>

IndentableStreamBuf::IndentableStreamBuf(
        std::streambuf* underlying_streambuf,
        std::size_t shift_size):
    underlying_streambuf(underlying_streambuf),
    shift_size(shift_size)
{};

void IndentableStreamBuf::shift(Direction direction)
{
    switch(direction)
    {
    case Direction::LEFT:
        shiftLeft();
        break;
    case Direction::RIGHT:
        shiftRight();
        break;
    }
}

void IndentableStreamBuf::shiftLeft()
{
    if (indent.size() >= shift_size)
        indent.resize(indent.size() - shift_size);
}

void IndentableStreamBuf::shiftRight()
{
    indent.append(shift_size, ' ');
}

std::streamsize IndentableStreamBuf::xsputn(const char_type* s, std::streamsize count)
{
    std::streamsize number_character_written = 0;

    const char_type* newline_character = traits_type::find(s, count, '\n');
    std::size_t current_part_size;
    while (newline_character != NULL) {
        current_part_size = std::distance(s, newline_character) + 1;
        number_character_written += 
            sputWithIndentAtBeginningIfNeeded(s, current_part_size);
        need_to_indent = true;

        std::advance(s, current_part_size);
        count -= current_part_size;
        newline_character = traits_type::find(s, count, '\n');
    }
    number_character_written += sputWithIndentAtBeginningIfNeeded(s, count);
    return number_character_written;
}

std::streamsize IndentableStreamBuf::sputWithIndentAtBeginningIfNeeded(
    const char_type* s,
    std::streamsize count)
{
    if (count > 0)
        sputIndentIfNeeded();
    return underlying_streambuf->sputn(s, count);
}

IndentableStreamBuf::int_type IndentableStreamBuf::overflow(int_type ch)
{
    sputIndentIfNeeded();
    return underlying_streambuf->sputc(ch);
};

void IndentableStreamBuf::sputIndentIfNeeded()
{
    if (need_to_indent)
    {
        underlying_streambuf->sputn(indent.c_str(), indent.size());
        need_to_indent = false;
    }
}

Shifter shift(IndentableStreamBuf::Direction direction)
{
    return Shifter{direction};
}


std::ostream& shiftLeft(std::ostream& os)
{
    os << shift(IndentableStreamBuf::Direction::LEFT);
    return os;
}

std::ostream& shiftRight(std::ostream& os)
{
    os << shift(IndentableStreamBuf::Direction::RIGHT);
    return os;
}

Shifter::Shifter(IndentableStreamBuf::Direction direction):
    direction(direction)
{};

std::ostream& operator<<(std::ostream& os, const Shifter& shifter)
{
    IndentableStreamBuf* buf = dynamic_cast<IndentableStreamBuf*>(os.rdbuf());
    if (buf == nullptr)
        throw std::invalid_argument("The stream must have an IndentableStreamBuf");
    buf->shift(shifter.direction);
    return os;
}

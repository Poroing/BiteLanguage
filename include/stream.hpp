#ifndef _H_BRAXOSTREAM_H_
#define _H_BRAXOSTREAM_H_

#include <ostream>
#include <iostream>

//TODO: Try to look into making a one character string for optimisation

class IndentableStreamBuf : public std::streambuf
{
public:
    enum class Direction { RIGHT, LEFT };

    typedef std::streambuf inherited;

    IndentableStreamBuf() = delete;
    IndentableStreamBuf(const IndentableStreamBuf& other) = default;
    explicit IndentableStreamBuf(
        std::streambuf* underlying_streambuf,
        std::size_t shift_size);

    void shift(Direction direction);
    void shiftRight();
    void shiftLeft();

protected:
    std::streamsize xsputn(const char_type* s, std::streamsize count) override;
    int_type overflow(int_type ch = traits_type::eof()) override;

private:
    
    std::streamsize sputWithIndentAtBeginningIfNeeded(
        const char_type* s,
        std::streamsize count);
    void sputIndentIfNeeded();

    std::string indent;
    const std::size_t shift_size;
    //An indent is needed if the last character is a newline character
    bool need_to_indent = true;
    std::streambuf* underlying_streambuf;
};

class Shifter;

std::ostream& shiftLeft(std::ostream& os);
std::ostream& shiftRight(std::ostream& os);
Shifter shift(IndentableStreamBuf::Direction direction);
    
class Shifter
{
    Shifter(IndentableStreamBuf::Direction direction);
    IndentableStreamBuf::Direction direction;
    friend std::ostream& operator<<(std::ostream& os, const Shifter& shifter);
    friend Shifter shift(IndentableStreamBuf::Direction direction);
};

#endif


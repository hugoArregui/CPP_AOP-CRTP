/*
    Copyright (C) 2011 Hugo Arregui

    This file is part of the "CPP: AOP + CRTP" Library.

    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. The name of the authors may not be used to endorse or promote products
       derived from this software without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <cmath>
#include "aop.h"

//#define INHERITING_CTORS  as of g++ 6.4.3, inheriting ctors was not implemented

template <typename _UnderlyingType>
struct Number
{
    template <template <class> class A = aop::NullAspect>
    class Type
    {
    public:
        typedef _UnderlyingType UnderlyingType;
        typedef A<typename Number::template Type<A>> FullType;

        Type(UnderlyingType n)
            : n(n)
        {}

        friend std::ostream& operator<<(std::ostream& out, const Type& number)
        {
            return out << number.n;
        }
    protected:
        UnderlyingType n;
    };
};

template <class A>
class ArithmeticAspect: public A
{
public:
    typedef typename A::FullType FullType;

#ifdef INHERITING_CTORS
    using A::A;
#else
    ArithmeticAspect(typename A::UnderlyingType n)
        : A(n)
    {}

    ArithmeticAspect(const A& a)
        : A(a)
    {}
#endif

    FullType operator+(const FullType& other) const
    {
        FullType tmp(*this);
        return tmp += other;
    }

    FullType operator-(const FullType& other) const
    {
        FullType tmp(*this);
        return tmp -= other;
    }

    FullType operator+=(const FullType& other)
    {
        A::n += other.n;
        return A::n;
    }

    FullType operator-=(const FullType& other)
    {
        A::n -= other.n;
        return A::n;
    }

    // same for *, *=, /, /=
};

template <class A>
class IncrementalAspect: public A
{
public:
    typedef typename A::FullType FullType;

#ifdef INHERITING_CTORS
    using A::A;
#else
    IncrementalAspect(typename A::UnderlyingType n)
        : A(n)
    {}

    IncrementalAspect(const A& a)
        : A(a)
    {}
#endif

    FullType operator++(int)
    {
        FullType tmp(*this);
        operator++();
        return tmp;
    }

    FullType operator++()
    {
        ++A::n;
        return *this;
    }

    FullType operator--(int)
    {
        FullType tmp(*this);
        operator--();
        return tmp;
    }

    FullType operator--()
    {
        --A::n;
        return *this;
    }
};

/*
* Configurable Aspect sumExample
*/
template <unsigned int PRECISION>
struct RoundAspect
{
    template <class A>
    class Type : public A
    {
    public:
        typedef typename A::FullType FullType;

#ifdef INHERITING_CTORS
        using A::A;
#else
        Type(typename A::UnderlyingType n)
            : A(n)
        {}

        Type(const A& a)
            : A(a)
        {}
#endif

        FullType operator+(const FullType& other) const
        {
            return FullType(round(A::operator+(other).n));
        }

    private:
        static float round(float f)
        {
            const unsigned int e = std::pow(10, PRECISION);
            return float(int(f * e)) / e;
        }
    };
};

template <class A>
class LogicalAspect: public A
{
public:
    typedef typename A::FullType FullType;

#ifdef INHERITING_CTORS
    using A::A;
#else
    LogicalAspect(typename A::UnderlyingType n)
        : A(n)
    {}

    LogicalAspect(const A& a)
        : A(a)
    {}
#endif

    bool operator!() const
    {
        return !A::n;
    }

    bool operator&&(const FullType& other) const
    {
        return A::n && other.n;
    }

    bool operator||(const FullType& other) const
    {
        return A::n || other.n;
    }
};

template <class A>
class BitwiseAspect: public A
{
public:
    typedef typename A::FullType FullType;

#ifdef INHERITING_CTORS
    using A::A;
#else
    BitwiseAspect(typename A::UnderlyingType n)
        : A(n)
    {}

    BitwiseAspect(const A& a)
        : A(a)
    {}
#endif

    bool operator~() const
    {
        return ~A::n;
    }

    FullType operator&(const FullType& mask) const
    {
        return A::n & mask.n;
    }

    FullType operator|(const FullType& mask) const
    {
        return A::n | mask.n;
    }

    FullType operator<<(const FullType& bitcount) const
    {
        return A::n << bitcount.n;
    }

    FullType operator>>(const FullType& bitcount) const
    {
        return A::n >> bitcount.n;
    }

    FullType& operator>>=(const FullType& bitcount)
    {
        A::n >>= bitcount.n;
        return *static_cast<FullType*>(this);
    }
};

template <class N>
void sumExample(typename N::UnderlyingType n1, typename N::UnderlyingType n2)
{
    N a(n1);
    N b(n2);
    N c = a + b;
    std::cout << c << std::endl;
}

template <class N>
void orExample(typename N::UnderlyingType n1, typename N::UnderlyingType n2)
{
    N a(n1);
    N b(n2);
    std::cout << (a || b) << std::endl;
}

template <class N>
void bitwiseExample(typename N::UnderlyingType n1, typename N::UnderlyingType n2)
{
    N a(n1);
    N b(n2);
    std::cout << (a + ((b >>= 1) << 3)) << std::endl;
}

int main()
{

    typedef aop::Decorate<Number<unsigned int>::Type>::with<ArithmeticAspect, IncrementalAspect, LogicalAspect, BitwiseAspect>::Type IntegralNumber;
    bitwiseExample<IntegralNumber>(1, 2);
    sumExample<IntegralNumber>(1, 2);

    typedef aop::Decorate<Number<float>::Type>::with<RoundAspect<2>::Type, ArithmeticAspect, LogicalAspect>::Type FloatRoundLogicalNumber;
    orExample<FloatRoundLogicalNumber>(1, 0);

    typedef aop::Decorate<Number<int>::Type>::with<LogicalAspect>::Type IntLogicalNumber;
    orExample<IntLogicalNumber>(1, 0);

    typedef aop::Decorate<Number<float>::Type>::with<RoundAspect<2>::Type, ArithmeticAspect>::Type FloatRoundNumber;
    sumExample<FloatRoundNumber>(1.339, 1.1233);

    return 0;
}

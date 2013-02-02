/*
    Copyright (C) 2011-2012 Hugo Arregui

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

#ifndef AOP_H
#define AOP_H

namespace aop
{

template <class A>
class NullAspect
{};

template <template <template <class> class> class Base>
struct Decorate
{
private:
    struct None {};

    template <template <class> class ... Aspects>
    struct Apply;

    template <template <class> class T>
    struct Apply<T>
    {
        template <class E>
        using Type = T<E>;
    };

    template<template <class> class A1, template <class> class ... Aspects>
    struct Apply<A1, Aspects...>
    {
        template <class T>
        using Type = A1<typename Apply<Aspects...>::template Type<T>>;
    };

public:
    template<template <class> class ... Aspects>
    struct with
    {
        template <class T>
        using AspectsCombination = typename Apply<Aspects...>::template Type<T>;

        typedef AspectsCombination<Base<AspectsCombination>> Type;
    };
};
}
#endif

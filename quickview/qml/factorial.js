.pragma library

// CAUTION: shared library can't access QML component instance objects or properties directly

var factorialCount = 0

function factorial(a) {
    a = parseInt(a)

    if (0 < a)
        return a * factorial(a - 1)

    factorialCount += 1

    return 1
}

function factorialCallCount() {
    return factorialCount
}

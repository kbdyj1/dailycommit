import QtQuick

// Use async, event-driven programming
// Use work threads
// Never manually spin the event loop
// Never spend more than a couple of milliseconds per frame within blocking functions

// JavaScript Code
// dynamic function, signal handler, binding expression
// ensure that unnecessary processing isn't triggered accidentally

// Type-Conversion
// QVariantMap(c++) to variant(qml) expensive. Use var
// QList of int, qreal, bool, QString, QUrl (inexpensive)
// other list types involve an expensive conversion cost(creating a new JavaScript Array, and adding new types one by one, with per-type conversion (c++ -> JavaScript)
// Using the closest matching property type
//

// Other JavaScript Objects
// Avoid using eval()
// Do not delete properties of objects

// Text Elements
// Prefer PlaneText
// Do not use RichText

// Images
// Use Asynchronous Loading
// Explcit Source Size. changing sourceSize will cause the image to be reloaded

// Avoid Run-time Composition(effect)

// Avoid Smoothing Images

// Position Elements with Anchors
// It is more efficient  to use anchors rather than bindings to position items relative to each other

// Models and Views

// Custom C++ Models
// Be as asynchronous as possible
// Do all processing in a (low priority) worker thread
// Batch up backend operations so that I/O and IPC is minimized
// Use a sliding slice window to cache results, whose parameters are determined with the help of profiling

// ListModel QML Type
// JavaScript engine is per-thread. increase memory usage
// Don't use dynamic roles
// if the type doesn't change, the caching performance improves dramatically

// Views
// View delegate should be kept as simple as possible.
// > The fewer elements that are in a delegate, the faster they can be created
// > Keep the number of bindings in a delegate to a minimum (use anchors rather than bindings)
// > Avoid using ShaderEffect elements within delegates
// > Never enabling clipping on a delegate
// set the cacheBuffer property

// Visual Effects
// Animations
// disconnect bindings during property animatioin
// Avoid running JavaScript during animation
// be careful using script animations, as these are run in the main thread


Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Item {
        id: root

        Loader {
            id: loader
            //source: "/PropertyBindings.qml"
            source: "/SequenceTips.qml"
        }
    }
}

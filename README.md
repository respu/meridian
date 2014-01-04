Meridian
* Home: http://crampton.me/meridian/
* Documentation: http://crampton.me/meridian/docs/

About Meridian
================================================================================

Meridian is a C++11/14 application framework specifically tailored for
server side networked applications. It's meant to be extremely
efficient in power usage, memory usage, and low-latency. It's very much a
work-in-progress.

Meridian is licensed under the permissive Boost Software License 1.0,
an OSI-approved and GPL compatible license.

Authors
--------------------------------------------------------------------------------

Eric Crampton (email: name of this project (at) crampton.me)

Principles
--------------------------------------------------------------------------------

*Meridian is unapologetically UNIX.* While I'm not anti-Windows, I'm
not an expert in multi-threaded, networked, C++ Windows programming
(but I've done UNIX network programming since the mid-90s!). Meridian
embraces the concept of the UNIX file descriptor, and makes (or will
when I've written the code) it easy to use non-socket concepts like
pipes, inotify descriptors, and anything else you can represent as a
file descriptor all in the same event loop. Meridian works on all free
UNIX-like operating systems (Linux, FreeBSD, and OpenBSD are
specifically tested).

*Meridian is unapologetically C++11/14.* Where it makes sense, newer
C++ features such as alias templates, lambdas, move construction,
smart pointer types, and library enhancements. Many of these features
to the language and library are new. I try to be careful about when
and how they're used as we all re-learn C++ best practices. Meridian
uses iostreams, RAII, and other familiar C++ concepts.

*Meridian tries not to hide too much from you.* Meridian treats you
like an adult and doesn't paper over some underlying types like
<tt>socklen_t</tt> or <tt>in_port_t</tt> by <tt>typedef</tt>'ing them
into something else for you. Instead, it exposes them without
apology. In some cases, Meridian defines an enumerated type instead of
forcing you to use poorly typed <tt>int</tt>s (e.g., an enumeration
for AF_INET, AF_INET6, and AF_UNIX).

*Meridian prefers to reuse other open projects.* Well, not in all
cases, or Meridian wouldn't exist. But, rather that, say, create a new
XML library just so it *feels* right, Meridian assumes you already
have a favorite and will use it. Of course, in some cases, Meridian
reimplements something which already exists to do it in a new and
unique way.

Alternatives
--------------------------------------------------------------------------------

There are lots of alternatives to Meridian which are more currently
more fully developed or are trying to solve different tasks. You
should probably be using one of these right now unless you want to
help build Meridian.

* <a href="http://pocoproject.org/">Poco Project</a>. A cross-platform
  framework (which works on Windows, too!). It's an entire world you
  can live in, complete with logging functionality, XML library,
  regular expression library, etc. Very mature documentation and a
  friendly API.

* <a
  href="http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio.html">Boost.asio</a>.
  Unlike Poco, Boost.asio is just about networking and that's it. It's
  fine, but suffers from the cross-platform lowest common denominator
  problem/advantage.

* <a href="http://www.cs.wustl.edu/~schmidt/ACE.html">ACE</a>. I used
  to use this one back in the late-90s. It was starting to feel a
  little old then, it's ancient now. Refuses to use anything modern in
  C++, but it is cross-platform and featureful.

Requirements
--------------------------------------------------------------------------------

* *A pretty darn modern C++ compiler.* Meridian is regularly built
   with clang++ 3.3 and gcc 4.8.2. It will work with *slightly* older
   versions of those two.

* *Boost.* Meridian is currently built against Boost 1.54, but
   probably works with slightly older versions. Meridian prefers to
   use C++11/14 concepts in place of Boost ones (e.g.,
   <tt>std::unique_ptr</tt> instead of <tt>boost::scoped_ptr</tt>).

Building
--------------------------------------------------------------------------------

Meridian is built using the <a
href="https://code.google.com/p/waf/">waf</a> build tool, which is
included in the repository. Simply run <tt>./waf configure</tt> and
then <tt>./waf</tt>. For more advanced usage, see the waf
documentation or <tt>./waf --help</tt>.

Documentation
--------------------------------------------------------------------------------

Meridian is documented using Doxygen. A Doxyfile is included in the
root which can be used to produce the documentation in HTML format.
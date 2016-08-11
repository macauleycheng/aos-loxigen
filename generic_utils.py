# Copyright 2013, Big Switch Networks, Inc.
#
# LoxiGen is licensed under the Eclipse Public License, version 1.0 (EPL), with
# the following special exception:
#
# LOXI Exception
#
# As a special exception to the terms of the EPL, you may distribute libraries
# generated by LoxiGen (LoxiGen Libraries) under the terms of your choice, provided
# that copyright and licensing notices generated by LoxiGen are not altered or removed
# from the LoxiGen Libraries and the notice provided below is (i) included in
# the LoxiGen Libraries, if distributed in source code form and (ii) included in any
# documentation for the LoxiGen Libraries, if distributed in binary form.
#
# Notice: "Copyright 2013, Big Switch Networks, Inc. This library was generated by the LoxiGen Compiler."
#
# You may not use this file except in compliance with the EPL or LOXI Exception. You may obtain
# a copy of the EPL at:
#
# http://www.eclipse.org/legal/epl-v10.html
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# EPL for the specific language governing permissions and limitations
# under the EPL.

"""
@brief Generic utilities

Intended to be imported into another namespace
"""
import logging
import collections
import functools
import sys

################################################################
#
# Debug
#
################################################################

def debug(obj):
    """
    Legacy logging method. Delegate to logging.debug.
    Use logging.debug directly in the future.
    """
    logging.debug(obj)

def log(obj):
    """
    Legacy logging method. Delegate to logging.info.
    Use logging.info directly in the future.S
    """
    logging.info(obj)

################################################################
#
# Memoize
#
################################################################

def memoize(obj):
    """ A function/method decorator that memoizes the result"""
    cache = obj.cache = {}

    @functools.wraps(obj)
    def memoizer(*args, **kwargs):
        key = args + tuple(kwargs.items())
        if key not in cache:
            cache[key] = obj(*args, **kwargs)
        return cache[key]
    return memoizer

################################################################
#
# OrderedSet
#
################################################################

class OrderedSet(collections.MutableSet):
    """
    A set implementations that retains insertion order.  From the receipe
    http://code.activestate.com/recipes/576694/
    as referred to in the python documentation
    """

    def __init__(self, iterable=None):
        self.end = end = []
        end += [None, end, end]         # sentinel node for doubly linked list
        self.map = {}                   # key --> [key, prev, next]
        if iterable is not None:
            self |= iterable

    def __len__(self):
        return len(self.map)

    def __contains__(self, key):
        return key in self.map

    def add(self, key):
        if key not in self.map:
            end = self.end
            curr = end[1]
            curr[2] = end[1] = self.map[key] = [key, curr, end]

    def discard(self, key):
        if key in self.map:
            key, prev, next = self.map.pop(key)
            prev[2] = next
            next[1] = prev

    def __iter__(self):
        end = self.end
        curr = end[2]
        while curr is not end:
            yield curr[0]
            curr = curr[2]

    def __reversed__(self):
        end = self.end
        curr = end[1]
        while curr is not end:
            yield curr[0]
            curr = curr[1]

    def pop(self, last=True):
        if not self:
            raise KeyError('set is empty')
        key = self.end[1][0] if last else self.end[2][0]
        self.discard(key)
        return key

    def __repr__(self):
        if not self:
            return '%s()' % (self.__class__.__name__,)
        return '%s(%r)' % (self.__class__.__name__, list(self))

    def __eq__(self, other):
        if isinstance(other, OrderedSet):
            return len(self) == len(other) and list(self) == list(other)
        return set(self) == set(other)

################################################################
#
# OrderedDefaultDict
#
################################################################

class OrderedDefaultDict(collections.OrderedDict):
    """
    A Dictionary that maintains insertion order where missing values
    are provided by a factory function, i.e., a combination of
    the semantics of collections.defaultdict and collections.OrderedDict.
    """
    def __init__(self, default_factory=None, *a, **kw):
        if (default_factory is not None and
                not callable(default_factory)):
            raise TypeError('first argument must be callable')
        collections.OrderedDict.__init__(self, *a, **kw)
        self.default_factory = default_factory

    def __getitem__(self, key):
        try:
            return collections.OrderedDict.__getitem__(self, key)
        except KeyError:
            return self.__missing__(key)

    def __missing__(self, key):
        if self.default_factory is None:
            raise KeyError(key)
        self[key] = value = self.default_factory()
        return value

    def __reduce__(self):
        if self.default_factory is None:
            args = tuple()
        else:
            args = self.default_factory,
        return type(self), args, None, None, self.items()

    def copy(self):
        return self.__copy__()

    def __copy__(self):
        return type(self)(self.default_factory, self)

    def __deepcopy__(self, memo):
        import copy
        return type(self)(self.default_factory,
                          copy.deepcopy(self.items()))
    def __repr__(self):
        return 'OrderedDefaultDict(%s, %s)' % (self.default_factory,
                                        collections.OrderedDict.__repr__(self))


def find(func, iterable):
    """
    find the first item in iterable for which func returns something true'ish.
    @returns None if no item in iterable fulfills the condition
    """
    for i in iterable:
        if func(i):
            return i
    return None

def count(func, iteratable):
    """
    count how the number of items in iterable for which func returns something true'ish.
    """
    c = 0
    for i in iterable:
        if func(i):
            c +=1
    return c

def chunks(l, n):
    """
    Yield successive n-sized chunks from l.
    From http://stackoverflow.com/questions/312443/how-do-you-split-a-list-into-evenly-sized-chunks-in-python
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

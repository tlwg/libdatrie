tests = [
    'walk',
    'iterator',
    'store-retrieve',
    'file',
    'serialization',
    'nonalpha',
    'null_trie',
    'term_state',
    'byte_alpha',
    'byte_list',
]

foreach item : tests
    exe = executable(
        'test_' + item,
        'test_@0@.c'.format(item),
        'utils.c',
        dependencies: libdatrie_dep,
    )
    test(item, exe)
endforeach

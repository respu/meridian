# -*- python -*-

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx boost waf_unit_test')

def configure(conf):
    conf.load('compiler_cxx boost waf_unit_test')
    conf.env.append_value('CXXFLAGS', ['-Wall', '-std=c++11', '-g'])
    conf.check_boost('date_time unit_test_framework')
    
def build(bld):
    bld.recurse('core reactor network')
    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)

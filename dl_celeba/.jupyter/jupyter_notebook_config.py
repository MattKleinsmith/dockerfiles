# Copyright 2015 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
import os
from IPython.lib import passwd

c.NotebookApp.ip = '*'
c.NotebookApp.port = int(os.getenv('PORT', 8888))
c.NotebookApp.open_browser = False
c.MultiKernelManager.default_kernel_name = 'python3'

# sets a password if PASSWORD is set in the environment
if 'PASSWORD' in os.environ:
  c.NotebookApp.password = passwd(os.environ['PASSWORD'])
  del os.environ['PASSWORD']

# Based off of https://github.com/jupyter/notebook/blob/master/docs/source/extending/savehooks.rst

import io
import os
from notebook.utils import to_api_path

_script_exporter = None
_html_exporter = None

def script_post_save(model, os_path, contents_manager, **kwargs):
    """convert notebooks to Python script after save with nbconvert

    replaces `ipython notebook --script`
    """
    from nbconvert.exporters.script import ScriptExporter
    from nbconvert.exporters.html import HTMLExporter

    if model['type'] != 'notebook':
        return

    global _script_exporter
    if _script_exporter is None:
        _script_exporter = ScriptExporter(parent=contents_manager)
    log = contents_manager.log

    global _html_exporter
    if _html_exporter is None:
        _html_exporter = HTMLExporter(parent=contents_manager)
    log = contents_manager.log

    # save .py file
    base, ext = os.path.splitext(os_path)
    script, resources = _script_exporter.from_filename(os_path)
    script_fname = base + resources.get('output_extension', '.txt')
    log.info("Saving script /%s", to_api_path(script_fname, contents_manager.root_dir))
    with io.open(script_fname, 'w', encoding='utf-8') as f:
        f.write(script)

    # save html
    base, ext = os.path.splitext(os_path)
    script, resources = _html_exporter.from_filename(os_path)
    script_fname = base + resources.get('output_extension', '.txt')
    log.info("Saving html /%s", to_api_path(script_fname, contents_manager.root_dir))
    with io.open(script_fname, 'w', encoding='utf-8') as f:
        f.write(script)
c.FileContentsManager.post_save_hook = script_post_save

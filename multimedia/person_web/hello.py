from flask import Flask
import os
from flask import Flask, request, redirect, url_for
from werkzeug import secure_filename

UPLOAD_FOLDER = './upload'
ALLOWED_EXTENSIONS = set(['mp4','mp3','zip','avi','mkv','txt', 'pdf', 'png', 'jpg', 'jpeg', 'gif'])

app = Flask(__name__, static_url_path='')
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS

# @app.route('/upload', methods=['GET', 'POST'])
# def upload_file():
#     if request.method == 'POST':
#         file = request.files['file']
#         if file and allowed_file(file.filename):
#             filename = secure_filename(file.filename)
#             file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
#             return redirect(url_for('uploaded_file',
#                                     filename=filename))
#     return '''
#     <!doctype html>
#     <title>Upload new File</title>
#     <h1>Upload new File</h1>
#     <form action="" method=post enctype=multipart/form-data>
#       <p><input type=file name=file>
#          <input type=submit value=Upload>
#     </form>
#     '''

from flask import send_from_directory
from flask import Flask, stream_with_context, url_for, Response, request, send_file
from StringIO import StringIO
from functools import wraps

import re
import mimetypes
import os
import cgi
import urllib2
PATTERN = re.compile('(\d{4}-\d{2}-\d{2})')


def check_auth(username, password):
    return username == 'username' and password == 'password'


def authenticate():
    return Response(
        u'auth failed', 401, {'WWW-Authenticate': 'Basic realm="Login Required"'}
    )


def requires_auth(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        auth = request.authorization
        if not auth or not check_auth(auth.username, auth.password):
            return authenticate()
        return f(*args, **kwargs)
    return decorated


def sort_filename(x):
    data = PATTERN.findall(x)
    if not data:
        return ''
    return data[0]


def show_dir_list(dir_path):
    f = StringIO()
    # list directory
    abs_path = os.path.relpath(dir_path, UPLOAD_FOLDER)
    if abs_path == '.':
        abs_path = ''
    f.write('<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">')
    f.write("<html>\n<title>Directory listing for /%s</title>\n" % abs_path)
    f.write("<body>\n<h2>Directory listing for /%s</h2>\n" % abs_path)
    f.write("<hr>\n<ul>\n")

    dir_list = os.listdir(dir_path)
    dir_list.sort(key=lambda x: sort_filename(x), reverse=True)
    for name in dir_list[:30]:
        fn = os.path.join(abs_path, name)
        uri = url_for('link', file_name=urllib2.quote(fn))
        f.write('<li><a href="%s">%s</a>\n'
                % (uri, cgi.escape(name)))
    f.write("</ul>\n<hr>\n</body>\n</html>\n")
    f.seek(0)
    return f

@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'],
                               filename)
@app.route('/', methods=['GET', 'POST'])
def main_html():
    if request.method == 'POST':
        file = request.files['file']
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            # return redirect(url_for('uploaded_file',
            #                         filename=filename))

    return app.send_static_file('thief_valley.html') 

@app.route('/download/<file_name>', methods=['GET'])
#@requires_auth
def link(file_name):
    file_name = urllib2.unquote(file_name)
    file_path = os.path.join(UPLOAD_FOLDER, file_name)
    if os.path.isdir(file_path) is True:
        resp = show_dir_list(file_path)
        return resp.read()

    if not os.path.exists(file_path):
        return 'File not exists!'

    def generate():
        f = open(file_path)
        for line in f:
            yield line

    mt, _ = mimetypes.guess_type(file_name)
    resp = Response(generate(), mimetype=mt)
    resp.headers.add('content-length', str(os.path.getsize(file_name)))
    return resp

@app.route('/download')
def downloadFile():
    f = show_dir_list(UPLOAD_FOLDER)
    return f.read()

if __name__ == '__main__':
    app.run()

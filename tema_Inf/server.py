from flask import Flask, request, render_template, redirect, session
import os
from werkzeug.utils import secure_filename
from PIL import Image    

# Note: static folder means all files from there will be automatically served over HTTP
app = Flask(__name__, static_folder="public", static_url_path="/public")
app.secret_key = "nuItiDau"

UPLOAD_FOLDER = 'public/upload/'
ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg', 'gif'}

# TODO Task 02: you can use a global variable for storing the auth session
# e.g., add the "authenticated" (boolean) and "username" (string) keys.
session = {
    "username": "Guest",
    "authenticated": False,
}

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


# you can use a dict as user/pass database
ALLOWED_USERS = {
    "test": "test123",
    "admin": "admin",
    "username": "password",
}

# Task 04: database filename
DATABASE_FILE = "database.txt"


@app.route('/')
def index():
    categories = ['Nature', 'People']
    images_Nature = []
    img = os.listdir('public/upload/Nature')
    for i in img:
        if ".thumb" in i:
            x = "public/upload/Nature/" + i
            tmp = {"tmb": x, "img": x.replace(".thumb", "")}
            images_Nature.append(tmp)
    images_People = []
    img = os.listdir('public/upload/People')
    for i in img:
        if ".thumb" in i:
            x = "public/upload/People/" + i
            tmp = {"tmb": x, "img": x.replace(".thumb", "")}
            images_People.append(tmp)
    return render_template("index.html", username=session["username"], authenticated=session["authenticated"], nature=images_Nature, people=images_People)

@app.route("/display")
def display():
    img = request.args.get("img")
    if img is None:
        return redirect("/")
    return render_template("display.html", username=session["username"], authenticated=session["authenticated"], img=img)

@app.route("/edit")
def second():
    if (session["authenticated"] == False):
        return redirect("/login")
    return render_template("edit.html", username=session["username"], authenticated=session["authenticated"])

# TODO Task 02: Authentication
@app.route("/login", methods=["GET", "POST"])
def login():
    error_msg = ""
    if request.method == "POST":
        username = request.form.get("username", "")
        password = request.form.get("password", "")
        if username in ALLOWED_USERS and ALLOWED_USERS[username] == password:
            session["username"] = username
            session["authenticated"] = True
            return redirect("/edit")
        else:
            error_msg = "Invalid username or password. Please try again."
    # return render_template("login.html", error_msg=error_msg)
    return render_template("login.html", error_msg=error_msg)

@app.route("/logout")
def logout():
    # TODO Task 02: clear authentication status
    #session["authenticated"] = False
    #return redirect("/index.html")
    session["username"] = "Guest"
    session["authenticated"] = False
    return render_template("index.html", username=session["username"], authenticated=session["authenticated"])

@app.route("/about")
def about():
    return render_template("about.html", username=session["username"], authenticated=session["authenticated"])

@app.context_processor
def inject_template_vars():
    return {
        "todo_var": "TODO_inject_common_template_variables"
    }


# You can use this as a starting point for Task 04
# (note: you need a "write" counterpart)
def read_database(username):
    """ Reads the user account details database file (line by line). """
    filename = f"data/{username}/user_details"
    with open(filename, "rt") as f:
        line1 = f.readline()
        line2 = f.readline()
        age = int(f.readline())
        return {
            "first_name": line1,
            "last_name": line2,
            "age": age,
        }
        
def write_database(username, first_name, last_name, age):
    """ Writes the user account details to the database file. """
    filename = f"data/{username}/user_details"
    with open(filename, "wt") as f:
        f.write(first_name + "\n")
        f.write(last_name + "\n")
        f.write(str(age) + "\n")
    return "Details Saved Successfully"
    

# TODO Task 04: Save Account Details
@app.route("/account-details", methods=["GET", "POST"])
def save_account():
    # Hint: if method == "GET", read the data from the database and pass it to the template
    # otherwise (when POST), replace the database with the user-provided data.
    if not session["authenticated"]:
        return redirect("/login")
    msg = ""
    if request.method == "POST":
        first_name = request.form.get("first_name", "")
        last_name = request.form.get("last_name", "")
        age = request.form.get("age", 0)
        msg=write_database(session["username"], first_name, last_name, age)
        user = read_database(session["username"])
    else:
        user = read_database(session["username"])
    return render_template("account-details.html", username=session["username"], authenticated=session["authenticated"], user=user, msg=msg)

@app.route('/upload', methods=['POST'])
def upload_file():
    # check if the post request has the file part
    if 'image' not in request.files:
        return 'No file part', 400
    file = request.files['image']
    # if user does not select file, browser also
    # submit an empty part without filename
    if file.filename == '':
        return 'No selected file', 400
    if file and allowed_file(file.filename):
        filename = secure_filename(file.filename)
        # You can now also save the name and category to your database
        category = request.form.get('category')
        # Check if category is valid
        if category not in ['People', 'Nature']:
            return category, 400
        # Create category specific path
        category_path = os.path.join(UPLOAD_FOLDER, category)
        # Create category directory if not exists
        if not os.path.exists(category_path):
            os.makedirs(category_path)
        # Save the file to the category specific path
        file_path = os.path.join(category_path, filename)
        file.save(file_path)
        # Create a thumbnail
        image = Image.open(file_path)
        image.thumbnail((200, 200))
        # Save the thumbnail with a .thumb.<extension> suffix
        thumbnail_path = os.path.splitext(file_path)[0] + '.thumb' + os.path.splitext(file_path)[1]
        image.save(thumbnail_path)
        # TODO: Save name and category to database
        return render_template("edit.html", username=session["username"], authenticated=session["authenticated"]), 200
    else:
        return 'Allowed file types are png, jpg, jpeg, gif', 400

@app.errorhandler(404)
def error404(code):
    # bonus: make it show a fancy HTTP 404 error page, with red background and bold message ;) 
    return "HTTP Error 404 - Page Not Found"


# Run the webserver (port 5000 - the default Flask port)
if __name__ == "__main__":
    app.run(host="0.0.0.0")


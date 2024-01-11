import os

# Directory structure to create
dir_structure = {
    '3rdParty': {
        'json': {}
    },
    'assets': {},
    'bin': {
        'demo': None
    },
    'build': {},
    'include': {
        'WordFrequencyAnalyzer.h': None,
        'DataStructures': {
            'IWordContainer.h': None,
            'ListWordContainer.h': None,
            'TreeWordContainer.h': None,
            'HashWordContainer.h': None
        },
        'Utilities': {
            'StringUtils.h': None
        }
    },
    'src': {
        'main.cpp': None,
        'WordFrequencyAnalyzer.cpp': None,
        'DataStructures': {
            'ListWordContainer.cpp': None,
            'TreeWordContainer.cpp': None,
            'HashWordContainer.cpp': None
        },
        'Utilities': {
            'StringUtils.cpp': None
        }
    },
    'CMakeLists.txt': None,
    'README.md': None
}

def create_dir_structure(base_path, structure):
    for name, content in structure.items():
        path = os.path.join(base_path, name)
        if content is None:
            # It's a file
            if not os.path.exists(path):
                # Create the file only if it doesn't exist
                with open(path, 'w') as file:
                    pass
        else:
            # It's a directory
            if not os.path.exists(path):
                os.makedirs(path)
            if isinstance(content, dict):
                # Recursively create the content of the directory
                create_dir_structure(path, content)

# Get the current working directory
current_directory = os.getcwd()

# Create the directory structure in the current working directory
create_dir_structure(current_directory, dir_structure)


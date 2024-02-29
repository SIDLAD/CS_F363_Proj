# write a script to parse ../grammar.txt and get all unique terms
# ignore ===>, | and for a term that starts with <, it ends with >, and the string in the middle is the term

import re
import os

def get_unique_terms():
    with open(os.path.join(os.path.dirname(__file__), "../grammar.txt"), "r") as f:
        lines = f.readlines()
        terms = set()
        not_terms = set()
        tokens = set()
        for line in lines:
            line = line.strip().split(" ")
            for term in line:
                if term.startswith("<") and term.endswith(">"):
                    term_ = term[1:-1]
                    not_terms.add(term_)
                    # check that all terms are either numerics, uppercase or _
                elif term == "|":
                    continue
                elif term == "===>":
                    continue
                else:
                    term_ = term
                    if not re.match(r'^[A-Z_0-9]+$', term_):
                        raise Exception(f"Term {term_} is not valid")
                    tokens.add(term_)
                
                # if term_ not in terms:
                terms.add(term_)
        return terms, not_terms, tokens
    
if __name__ == "__main__":
    terms, not_terms, tokens = get_unique_terms()
    print(len(terms))
    with open(os.path.join(os.path.dirname(__file__), "./unique_terms.txt"), "w") as f:
        for term in terms:
            f.write(term + "\n")
    
    with open(os.path.join(os.path.dirname(__file__), "./not_terms.txt"), "w") as f:
        for term in not_terms:
            f.write(term + "\n")
    
    with open(os.path.join(os.path.dirname(__file__), "./tokens.txt"), "w") as f:
        for term in tokens:
            f.write(term + "\n")

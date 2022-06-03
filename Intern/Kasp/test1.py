import json


class StepTreeRoot:
    def __init__(self):
        self.step_tree_json = {
            "CurrentTrees": [
                {
                    "RootSteps": []
                }
            ]
        }

    def bind_step(self, step_name):
        step = Step(step_name)
        self.step_tree_json["CurrentTrees"][0]["RootSteps"].append(step.__dict__)
        return step

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        print(json.dumps(self.step_tree_json).replace("\"", "\'"))
        if exc_val:
            raise exc_val


class Step:
    def __init__(self, step_name):
        self.Childs = []
        self.IsBreaking = 'false'
        self.StepTitle = step_name
        self.ExceptionMessage = ""

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.end(exc_val)

    def bind_step(self, step_name):
        step = Step(step_name)
        self.Childs.append(step.__dict__)
        return step

    def end(self, exc_val):
        self.StepTitle = ""
        if exc_val:
            self.IsBreaking = 'true'
            self.ExceptionMessage = str(exc_val)
            raise exc_val


try:
    with StepTreeRoot() as step_tree:
        with step_tree.bind_step("1") as step:
            with step.bind_step("1.1"):
                pass

            with step.bind_step("1.2"):
                pass

        with step_tree.bind_step("2") as step:
            with step.bind_step("2.1"):
                pass

        with step_tree.bind_step("3"):
            raise Exception("Exception message")

except Exception as e:
    pass
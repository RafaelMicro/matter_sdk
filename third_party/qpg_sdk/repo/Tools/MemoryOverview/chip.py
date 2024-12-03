try:
    from gppy.tools.memory.categorization.generic import Categorization
    import gppy.tools.memory.categorization.feature.matter as Matter
except ImportError:
    try:
        from generic import Categorization
        import matter as Matter
    except ImportError:
        print("Using outside of Env ?")


class CategorizationCHIP(Categorization):
    order = [
        "Thread", "CHIP", "Thread/CHIP glue",
        "BLE Base", "BLE", "Mesh",
    ] + Categorization.order

    Categorization.files.update({
        "CHIP": [],
        "Thread/CHIP glue": Matter.glue_files,
        "Application": Matter.application_files + Matter.cluster_files,
    })

    Categorization.default.update({
        "CHIP": {
            'P236_CHIP': {},
        },
        "Thread/CHIP glue": {
        },
    })

    Categorization.archives.update({
        "Thread/CHIP glue": {"libOpenThread_qpg61": [],
                             "libOpenThreadQorvoGlue": [],
                             },
        "Application": {"libZapGenerated": [], },
    })

    Categorization.combined_archives += ["libCHIP", "libCHIP_qpg61", "libCHIP_qpg6105", "libMatter"]

    Categorization.ignore_folders += ["lib"]  # P236_qvCHIP library
